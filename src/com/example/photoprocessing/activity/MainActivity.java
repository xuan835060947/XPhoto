package com.example.photoprocessing.activity;

import java.io.File;
import java.io.IOException;

import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.hardware.Camera.CameraInfo;
import android.net.Uri;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.photoprocessing.R;
import com.example.photoprocessing.sharedpool.ImageShare;
import com.example.photoprocessing.util.ImageUtil;
import com.example.photoprocessing.util.ToastUtil;
import com.example.photoprocessing.value.FinalValue;

public class MainActivity extends BaseActivity {

	private ScaleAnimation mScaleAnimation;
	private ImageView ivFolder, ivCamera;
	private TextView tvHome;

	final int SELECT_PIC_KITKAT = 0;

	private final int CIRCLE_BLUZ = 1001;
	private final int CAMERA_FOR_PHOTO = 1002;
	private final int FOLDER_FOR_PHOTO = 1003;

	private File cameraFile;
	private String imgPath;
	private int startActivity = 0;

	@Override
	public int getLayoutId() {
		// TODO Auto-generated method stub
		return R.layout.activity_main;
	}

	@Override
	public void setUI() {
		// TODO Auto-generated method stub
		ivFolder = (ImageView) mFindViewAndSetOnClick(R.id.ivFolder);
		ivCamera = (ImageView) mFindViewAndSetOnClick(R.id.ivCamere);

		mScaleAnimation = new ScaleAnimation(0.0f, 3.4f, 0.0f, 3.4f,
				Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
				0.5f);
		mScaleAnimation.setDuration(1000);
		mScaleAnimation.setFillAfter(true);
		tvHome = (TextView) findViewById(R.id.tvHome);
		tvHome.startAnimation(mScaleAnimation);

	}

	@Override
	public void OnClickListener(View view) {
		// TODO Auto-generated method stub
		switch (view.getId()) {
		case R.id.ivFolder:
			Intent intent = new Intent(Intent.ACTION_GET_CONTENT);// ACTION_OPEN_DOCUMENT
			intent.addCategory(Intent.CATEGORY_OPENABLE);
			intent.setType("image/jpeg");
			// if (android.os.Build.VERSION.SDK_INT >=
			// android.os.Build.VERSION_CODES.KITKAT) {
			// startActivityForResult(intent, SELECT_PIC_KITKAT);
			// } else {
			startActivityForResult(intent, FOLDER_FOR_PHOTO);
			// }
			break;
		case R.id.ivCamere:
			File file = new File(FinalValue.CAMERA_FOLDER);
			file.mkdirs();
			imgPath = android.os.Environment.getExternalStorageDirectory()
					.getAbsolutePath() + "/XPhoto/xuan.jpg";

			Intent cameraIntent = new Intent(MainActivity.this,CameraActivity.class);
			cameraFile = new File(imgPath);
			try {
				cameraFile.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			cameraIntent.putExtra("saveFolder", FinalValue.CAMERA_FOLDER);
			startActivityForResult(cameraIntent, CAMERA_FOR_PHOTO);
			break;
		}
	}

	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (resultCode == RESULT_OK) {
			Intent intent = new Intent(MainActivity.this,
					CircleBluzActivity.class);
			switch (requestCode) {
			case CAMERA_FOR_PHOTO: {
				ImageShare.imgPath = data.getStringExtra("path");
				Log.v(TAG, imgPath + "文件:" + ImageShare.imgPath);
				startActivity = CIRCLE_BLUZ;
				startActivity(intent);
			}
				break;
			case FOLDER_FOR_PHOTO:

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

				if (path != null && path.endsWith(".jpg")) {
					ImageShare.imgPath = path;
					startActivity = CIRCLE_BLUZ;
					startActivity(intent);
				} else {
					startActivity = 0;
					ToastUtil.show(this, "请选择jpeg图片");
				}

				break;
			}
		}
	};

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		// switch(startActivity){
		// case CIRCLE_BLUZ:
		// Intent intent = new Intent(MainActivity.this,
		// SurfaceProcessingActivity.class);
		// startActivity(intent);
		// break;
		// default:
		// break;
		//
		// }
	}

}