package com.example.photoprocessing.activity;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.photoprocessing.R;
import com.example.photoprocessing.component.SeekBarComtrol;
import com.example.photoprocessing.component.TouchCircle;
import com.example.photoprocessing.sharedpool.ImageShare;
import com.example.photoprocessing.util.FileUtil;
import com.example.photoprocessing.util.ToastUtil;
import com.example.photoprocessing.value.FinalValue;

/**
 * @author xuan 2014-10-30
 * 
 */
@SuppressLint("NewApi")
public class CircleBluzActivity extends BaseActivity {
	private final int SAVE_FINISH = 1000;
	private final int SAVE_START = 1001;

	private SurfaceView svShow;
	private SurfaceHolder svHolder;
	private TouchCircle tcShow;
	private String imgPath;
	private ImageView ivConfirm, ivCancel, ivReturn, ivAdd, ivReduce;
	private SeekBarComtrol sbcProgress;
	private TextView tvSave;

	@Override
	public int getLayoutId() {
		// TODO Auto-generated method stub
		if (ImageShare.imgPath != null) {
			imgPath = ImageShare.imgPath;
			FileUtil.saveInfo("图片路径:" + imgPath);
		}else{
			FileUtil.saveInfo("imgPath is null");
		}
		return R.layout.activity_circlebluz;
	}

	@Override
	public void setUI() {
		// TODO Auto-generated method stub
		svShow = (SurfaceView) mFindViewAndSetOnClick(R.id.svShow);
		svHolder = svShow.getHolder();
		tcShow = (TouchCircle) mFindViewAndSetOnClick(R.id.tcShow);
		tcShow.setAlpha(80);
		sbcProgress = (SeekBarComtrol) mFindViewAndSetOnClick(R.id.sbcProgress);
		ivReturn = (ImageView) mFindViewAndSetOnClick(R.id.ivReturn);
		tvSave = (TextView) mFindViewAndSetOnClick(R.id.tvSave);

		ivAdd = (ImageView) mFindViewAndSetOnClick(R.id.ivAddBluz);
		ivReduce = (ImageView) mFindViewAndSetOnClick(R.id.ivReduceBluz);

		svHolder.addCallback(new SurfaceHolder.Callback() {

			public void surfaceChanged(SurfaceHolder holder, int format,
					int width, int height) {
				Log.v(TAG, "surfaceChanged format=" + format + ", width="
						+ width + ", height=" + height);
				if (ImageShare.imgPath != null) {
					imgPath = ImageShare.imgPath;
					showImg(svHolder.getSurface(), imgPath);
				}
			}

			public void surfaceCreated(SurfaceHolder holder) {
				Log.v(TAG, "surfaceCreated");
			}

			public void surfaceDestroyed(SurfaceHolder holder) {
				Log.v(TAG, "surfaceDestroyed");
			}

		});

		ivConfirm = (ImageView) mFindViewAndSetOnClick(R.id.ivConfirm);
		ivCancel = (ImageView) mFindViewAndSetOnClick(R.id.ivCancel);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		destroyNativeAll();
	}

	@Override
	public void OnClickListener(View view) {
		// TODO Auto-generated method stub
		switch (view.getId()) {
		case R.id.ivCancel:
			new Thread() {
				public void run() {
					showCurImg(svHolder.getSurface());
				};
			}.start();
			break;
		case R.id.ivConfirm:
			new Thread() {
				public void run() {
					circleBluzShow(svHolder.getSurface(),
							(int) (tcShow.getX()), (int) (tcShow.getY()),
							tcShow.getRadius(), sbcProgress.getProgress());
				};
			}.start();
			break;

		case R.id.ivReturn:
			this.finish();
			break;
		case R.id.tvSave:
			new Thread() {
				public void run() {
					myHandler.sendEmptyMessage(SAVE_START);
					File file = new File(FinalValue.BLUZ_SAVE_FOLDER);
					file.mkdirs();
					Date date = new Date();
					SimpleDateFormat sf = new SimpleDateFormat("yyyyMMddhhmmss");
					String name = sf.format(date);
					String filePath = FinalValue.BLUZ_SAVE_FOLDER + "/" + name
							+ ".jpg";
					Log.v(TAG, name);
					circleBluz(filePath, (int) tcShow.getX(),
							(int) tcShow.getY(), tcShow.getRadius(),
							sbcProgress.getProgress());
					Message msg = new Message();
					msg.what = SAVE_FINISH;
					Bundle data = new Bundle();
					data.putString("filePath", filePath);
					msg.setData(data);
					myHandler.sendMessage(msg);

				};
			}.start();
			break;
		case R.id.ivAddBluz:
			sbcProgress.setProgress(sbcProgress.getProgress() + 1);
			break;
		case R.id.ivReduceBluz:
			sbcProgress.setProgress(sbcProgress.getProgress() - 1);
			break;

		}
	}

	Handler myHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			switch (msg.what) {
			case SAVE_FINISH:
				ToastUtil.show(CircleBluzActivity.this, "文件已保存为"
						+ msg.getData().getString("filePath"));
				break;
			case SAVE_START:
				ToastUtil.show(CircleBluzActivity.this, "正在保存图片");
				break;
			}
		}
	};

	
	
	static {
		System.loadLibrary("CircleBluz");
	}

	public native void showImg(Surface s, String imgPath);

	public native void showCurImg(Surface s);

	public native void circleBluz(String savePath, int x, int y, int radius,
			float sigma);

	public native void circleBluzShow(Surface s, int x, int y, int radius,
			float sigma);

	public native void destroyNativeAll();

}
