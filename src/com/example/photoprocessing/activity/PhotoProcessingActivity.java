package com.example.photoprocessing.activity;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

import com.example.photoprocessing.R;
import com.example.photoprocessing.sharedpool.ImageShare;


/**
 * @author xuan
 * 2014-10-30
 * 
 */
public class PhotoProcessingActivity extends Activity{
	
	private Bitmap bmOriginal,bmGray;
	private ImageView iv;
	private Button btGray,btOpen;
	
	static{
		System.loadLibrary("PhotoProcessing");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_photo_processing);
		
		setup();
	}
	
	public void setup(){
		iv = (ImageView) findViewById(R.id.showPhoto);
		bmOriginal = ImageShare.mBitmap;
		iv.setImageBitmap(bmOriginal);
		
		btGray = (Button) findViewById(R.id.btGray);
		btGray.setOnClickListener(onclick);
		btOpen = (Button) findViewById(R.id.btOpen);
		btOpen.setOnClickListener(onclick);
		
	}
	
	OnClickListener onclick = new OnClickListener(){
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			switch(v.getId()){
			case R.id.btGray:
				bmGray = Bitmap.createBitmap(bmOriginal.getWidth(), bmOriginal.getHeight(), Config.ALPHA_8);
				grayPhoto(bmOriginal, bmGray);
				iv.setImageBitmap(bmGray);
				break;
			case R.id.btOpen:
				Intent intent = new Intent(PhotoProcessingActivity.this,
						SurfaceProcessingActivity.class);
				startActivity(intent);
				break;
			}
		}
	};
	
	public native void grayPhoto(Bitmap bmOriginal,Bitmap bmGray);
	
	 
}
