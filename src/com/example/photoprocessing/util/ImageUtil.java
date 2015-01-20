package com.example.photoprocessing.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import android.app.Activity;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.provider.MediaStore;
import android.util.Log;


/**
 * @author xuan
 * 2014-10-30
 * 
 */
public class ImageUtil {

	/**
	 * 读取指定路径中的图片 同时将其翻转对应角度
	 * 
	 * @param path
	 * @return
	 * @throws Exception
	 */
	public static Bitmap getImageBitmap(String path, int request_width,
			int request_height) throws Exception {
		File file = new File(path);
		int degree = readPictureDegree(file.getAbsolutePath());
		Bitmap bitmap = null;
		try {
			bitmap = rotaingImageView(degree,
					decodeFile(file, request_width, request_height, false));
		} catch (Throwable t) {
			t.printStackTrace();
		}
		return bitmap;
	}

	/**
	 * 按比例压缩图片
	 * 
	 * @param file
	 * @param requestWidth
	 * @param requestHeight
	 * @return
	 */
	public static Bitmap decodeFile(File file, int requestWidth,
			int requestHeight, boolean wrapToHeight) {
		try {
			// decode image size
			BitmapFactory.Options options = new BitmapFactory.Options();
			options.inJustDecodeBounds = true;
			BitmapFactory
					.decodeStream(new FileInputStream(file), null, options);

			// Find the correct scale value .It should be the power of 2.
			int width_tmp = options.outWidth, height_tmp = options.outHeight;
			
			if (wrapToHeight && width_tmp > height_tmp) {
				int scals = height_tmp / width_tmp;
				requestHeight = requestHeight * scals;
			}
			int scalse = 1;
			while (true) {
				if ((width_tmp / 2 < requestWidth)
						|| (height_tmp / 2 < requestHeight)) {
					break;
				}
				width_tmp /= 2;
				height_tmp /= 2;
				scalse *= 2;
			}

			// decode with inSampleSize
			BitmapFactory.Options requestOptions = new BitmapFactory.Options();
			requestOptions.inSampleSize = scalse;
			return BitmapFactory.decodeStream(new FileInputStream(file), null,
					requestOptions);
		} catch (Exception e) {
			Log.v(" ImageUtil ", " decodeFile 错误 ");
		}
		return null;
	}

	/**
	 * 根据URI获取图片物理路径
	 * 
	 * */
	public static String getAbsoluteImagePath(Uri uri, Activity activity) {
		String[] proj = { MediaStore.Images.Media.DATA };
		Cursor cursor = activity.managedQuery(uri, proj, null, null, null);
		int column_index = cursor
				.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
		cursor.moveToFirst();
		return cursor.getString(column_index);
	}

	/**
	 * 读取图片属性：旋转的角度
	 * 
	 * @param path
	 *            图片绝对路径
	 * @return degree旋转的角度
	 */
	public static int readPictureDegree(String path) {
		int degree = 0;
		try {
			ExifInterface exifInterface = new ExifInterface(path);
			int orientation = exifInterface.getAttributeInt(
					ExifInterface.TAG_ORIENTATION,
					ExifInterface.ORIENTATION_NORMAL);
			switch (orientation) {
			case ExifInterface.ORIENTATION_ROTATE_90:
				degree = 90;
				break;
			case ExifInterface.ORIENTATION_ROTATE_180:
				degree = 180;
				break;
			case ExifInterface.ORIENTATION_ROTATE_270:
				degree = 270;
				break;
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return degree;
	}

	/**
	 * 旋转图片文件的角度
	 * 
	 * @param path
	 */
	public static void rotaingFile(String path) {
		try {
			ExifInterface exifInterface = new ExifInterface(path);
			String degree = String.valueOf(ExifInterface.ORIENTATION_ROTATE_90);
			exifInterface.setAttribute(ExifInterface.TAG_ORIENTATION, degree);
			exifInterface.saveAttributes();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	/*
	 * 旋转图片
	 * 
	 * @param angle
	 * 
	 * @param bitmap
	 * 
	 * @return Bitmap
	 */
	public static Bitmap rotaingImageView(int angle, Bitmap bitmap) {
		// 旋转图片 动作
		Matrix matrix = new Matrix();
		matrix.postRotate(angle);
		// 创建新的图片
		Bitmap resizedBitmap = Bitmap.createBitmap(bitmap, 0, 0,
				bitmap.getWidth(), bitmap.getHeight(), matrix, true);
		return resizedBitmap;
	}

	/**
	 * 按正方形裁切图片
	 */
	public static Bitmap imageCropToSquare(Bitmap bitmap) {
		int w = bitmap.getWidth();
		int h = bitmap.getHeight();

		int wh = w > h ? h : w;// 裁切后所取的正方形区域边长

		int retX = w > h ? (w - h) / 2 : 0;// 基于原图，取正方形左上角x坐标
		int retY = w > h ? 0 : (h - w) / 2;

		// 下面这句是关键
		return Bitmap.createBitmap(bitmap, retX, retY, wh, wh, null, false);
	}

}