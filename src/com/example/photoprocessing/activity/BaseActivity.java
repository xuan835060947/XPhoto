package com.example.photoprocessing.activity;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;


public abstract class BaseActivity extends Activity {
	
	/**
	 * 
	 * 根据自己项目的Activity的共同要求,将一些常规操作固定下来
	 * 所有带m的方法与父类方法类似,但有不同,是为使用方便而写
	 * 
	 */
	protected String TAG = this.toString();
	private OnClickListener onClick;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		onClick = new OnClickListener() {
			@Override
			public void onClick(View view) {
				// TODO Auto-generated method stub
				OnClickListener(view);
			}
		};
		setContentView(getLayoutId());
		setUI();
		
	}
	
	public final View mFindViewAndSetOnClick(int id) {
		View view = findViewById(id);
		view.setOnClickListener(onClick);
		return view;
	}

	public final void mFindViewAndSetOnClick(View view) {
		view.setOnClickListener(onClick);
	}
	
	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		Log.v(TAG, "onStart");
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		Log.v(TAG, "onDestroy");
	}
	
	
	public abstract int getLayoutId();

	public abstract void setUI();

	public abstract void OnClickListener(View view);
}
