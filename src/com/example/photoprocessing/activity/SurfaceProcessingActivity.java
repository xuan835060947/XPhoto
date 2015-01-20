package com.example.photoprocessing.activity;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import com.example.photoprocessing.R;
import com.example.photoprocessing.sharedpool.ImageShare;

/**
 * @author xuan 2014-10-30
 * 
 */
public class SurfaceProcessingActivity extends Activity {
	String TAG = "SurfaceProcessing";
	final int SELECT_PIC_KITKAT = 0;
	final int SELECT_PIC = 1;

	static SurfaceView svShow;
	static SurfaceHolder svHolder;
	Bitmap bmOriginal, bmShow;
	Button btGray, btShowImg;
	// 选择的图片路径
	String imgPath;
	// 该activity是否从onActivityResult中返回
	boolean isOnActivityResult = false;

	static {
		System.loadLibrary("SurfaceProcessing");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_circlebluz);
		Log.v(this.toString(), "onCreate");
		bmOriginal = ImageShare.mBitmap;
//		setup();
	}

	public void setup() {
		btGray = (Button) findViewById(R.id.btGray);
		btGray.setOnClickListener(onclick);

		btShowImg = (Button) findViewById(R.id.btShow);
		btShowImg.setOnClickListener(onclick);

		svShow = (SurfaceView) findViewById(R.id.svShow);
		svHolder = svShow.getHolder();

		svHolder.addCallback(new SurfaceHolder.Callback() {

			public void surfaceChanged(SurfaceHolder holder, int format,
					int width, int height) {
				Log.v(TAG, "surfaceChanged format=" + format + ", width="
						+ width + ", height=" + height);
				
			}

			public void surfaceCreated(SurfaceHolder holder) {
				Log.v(TAG, "surfaceCreated");
				if (isOnActivityResult && imgPath != null) {
					showJPG(holder.getSurface(), imgPath);
				}
			}

			public void surfaceDestroyed(SurfaceHolder holder) {
				Log.v(TAG, "surfaceDestroyed");
			}

		});

	}

	OnClickListener onclick = new OnClickListener() {
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			Log.v("OnClick", "Onclick");
			switch (v.getId()) {
			case R.id.btGray:
				Log.v("OnClick", "btGray");
				//grayPhoto(svHolder.getSurface());
				new Thread(){
					public void run() {
						fuzzyImg("/sdcard/xuan.jpg","/sdcard/afile/xuan22.jpg");
					};
				}.start();
				break;

			case R.id.btShow:
				Intent intent = new Intent(Intent.ACTION_GET_CONTENT);// ACTION_OPEN_DOCUMENT
				intent.addCategory(Intent.CATEGORY_OPENABLE);
				intent.setType("image/jpeg");
				if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.KITKAT) {
					startActivityForResult(intent, SELECT_PIC_KITKAT);
				} else {
					startActivityForResult(intent, SELECT_PIC);
				}
				break;
			}
		}
	};

	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (resultCode == RESULT_OK) {

			if (requestCode == SELECT_PIC) {
				Uri originalUri = data.getData();
				String[] proj = { MediaStore.Images.Media.DATA };
				// 好像是android多媒体数据库的封装接口，具体的看Android文档
				Cursor cursor = managedQuery(originalUri, proj, null, null,
						null);
				// 按我个人理解 这个是获得用户选择的图片的索引值
				int column_index = cursor
						.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
				// 将光标移至开头 ，这个很重要，不小心很容易引起越界
				cursor.moveToFirst();
				// 最后根据索引值获取图片路径
				String path = cursor.getString(column_index);
				Log.v("图片路径: ", path);

				if (path.endsWith(".jpg")) {
					isOnActivityResult = true;
					imgPath = path;
				}
			}
		}
	};


	private Bitmap getReduceBitmap(Bitmap bitmap, int w, int h,
			boolean zoomBaseWidth) {
		int width = bitmap.getWidth();
		int hight = bitmap.getHeight();
		Matrix matrix = new Matrix();
		float wScake = ((float) w / width);
		float hScake = ((float) h / hight);
		if (zoomBaseWidth) {
			matrix.postScale(wScake, wScake);
		} else {
			matrix.postScale(hScake, hScake);
		}

		return Bitmap.createBitmap(bitmap, 0, 0, width, hight, matrix, true);
	}

	public native void grayPhoto(Surface surface);

	public native void showJPG(Surface surface, String img);
	
	public native void fuzzyImg(String originalImg,String newImg);

}
