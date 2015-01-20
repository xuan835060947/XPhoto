package com.example.photoprocessing.component;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

/**
 * @author xuan 2014-10-30
 * 
 */
public class SeekBarComtrol extends View {

	

	// 该View的宽,高
	private int width = 0, height = 0;

	private int max = 50;
	private int curProgress = 25;
	private float x;
	private float y;
	private int radius = 10;

	private Paint paint;
	private Canvas mcanvas;
	private Bitmap cacheBitmap;

	public SeekBarComtrol(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		setup();
	}

	public SeekBarComtrol(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		setup();
	}
	
	public SeekBarComtrol(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		// TODO Auto-generated constructor stub
		setup();
	}

	public void setup() {

		paint = new Paint();
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.FILL_AND_STROKE);
		// 设置画笔笔画宽度
		paint.setStrokeWidth(4);
		paint.setAntiAlias(true);
		paint.setDither(true);

	}

	public void onSureMeasure() {
		Log.v("onSureMeasure", "start!!");
		cacheBitmap = Bitmap.createBitmap(width, height, Config.ARGB_8888);
		mcanvas = new Canvas();
		mcanvas.setBitmap(cacheBitmap);
		y = height / 2;
		mcanvas.drawLine(0, y, width, y, paint);
		x =  curProgress/(float)max * width;
		Log.v("X : ", "  "+x + " width " + width + " height :"+height);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		float curX = event.getX();
		float curRate = curX / width;
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			curProgress = (int) (curRate * max);
			x = curRate * width;
			break;
		case MotionEvent.ACTION_MOVE:
			curProgress = (int) (curRate * max);
			x = curRate * width;
			break;
		}
		invalidate();
		return true;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		canvas.drawBitmap(cacheBitmap, 0, 0, paint);
		canvas.drawCircle(x, y, radius, paint);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		// widthMeasureSpec、heightMeasureSpec是有高16位模式和低16位size,所以看起来会很大
		// 获得模式
		Log.v("onMeasure", "start!!");
		int mode = MeasureSpec.getMode(widthMeasureSpec);
		// AT_MOST限制最大尺寸,EXACTLY则是确定好尺寸了,例如在xml文件中设置了大小
		if (mode == MeasureSpec.AT_MOST || mode == MeasureSpec.EXACTLY) {
			// 获得View实际大小
			width = MeasureSpec.getSize(widthMeasureSpec);
			height = MeasureSpec.getSize(heightMeasureSpec);
			onSureMeasure();
			// 以下方法是设置View大小的方法,除非想在代码中控制大小,不然不应该在这里填写大小
			// 如果希望其生效则不要调用super.onMeasure(widthMeasureSpec,
			// heightMeasureSpec);这句了
			// setMeasuredDimension(400, height);
		} else if (mode == MeasureSpec.UNSPECIFIED) { // View的大小不确定时
			Log.d("WOGU", "mode=UNSPECIFIED");
		}
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	}

	public void setProgress(int progress) {
		if (progress < 0 || progress > max)
			this.curProgress = 0;
		else{
			this.curProgress = progress;
		}
		x = (float)width * ((float)curProgress / max);
		Log.v("X : ", "  "+x + " width " + width + " height :"+height);
		invalidate();
	}

	public void setRadius(int r) {
		radius = r;
		invalidate();
	}

	public int getProgress() {
		return curProgress;
	}

	private void measureView(View child) {  
        ViewGroup.LayoutParams p = child.getLayoutParams();  
        if (p == null) {  
            p = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,  
                    ViewGroup.LayoutParams.WRAP_CONTENT);  
        }  
  
        int childWidthSpec = ViewGroup.getChildMeasureSpec(0, 0, p.width);  
        int lpHeight = p.height;  
        int lpWidth = p.width;
        int childHeightSpec;  
        if (lpHeight > 0) {  
            childHeightSpec = MeasureSpec.makeMeasureSpec(lpHeight,  
                    MeasureSpec.EXACTLY);  
            childWidthSpec = MeasureSpec.makeMeasureSpec(lpWidth,  
                    MeasureSpec.EXACTLY);  
        } else {  
            childHeightSpec = MeasureSpec.makeMeasureSpec(0,  
                    MeasureSpec.UNSPECIFIED);  
        }  
        child.measure(childWidthSpec, childHeightSpec);  
    }  
	
}
