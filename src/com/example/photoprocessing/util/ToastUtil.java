package com.example.photoprocessing.util;

import android.content.Context;
import android.widget.Toast;

/**
 * 自定义Toast显示
 * @author ZZJ
 *
 */
public class ToastUtil {
	private static Toast mToast;
	
	/**
	 * 显示Toast
	 * @param context
	 * @param text
	 */
	public static void show(Context context, String text){
		if(mToast == null){
			mToast = Toast.makeText(context, text, Toast.LENGTH_SHORT);
		}else{
			mToast.setText(text);
			mToast.setDuration(Toast.LENGTH_SHORT);
		}
		mToast.show();
	}
	
	/**
	 * 取消自定义Toast的显示
	 */
	public static void cancel(){
		if(mToast != null){
			mToast.cancel();
		}
	}
	
}
