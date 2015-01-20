package com.example.photoprocessing.component;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;


/**
 * @author xuan
 * 2014-10-30
 * 
 */
public class TouchCircle extends View{
	

	String TAG = "MyCircleView";
	private Paint paint;
	private float x0, y0, x1, y1;
	private float originx, originy;
	private int radius = 50;
	private int curdistance;
	private float left, top, right, bottom;
	private RectF rectf;
	
	public TouchCircle(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		setup();
	}

	public TouchCircle(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		setup();
	}
	
	public TouchCircle(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		// TODO Auto-generated constructor stub
		setup();
	}

	public void setup() {
		paint = new Paint();
		paint.setColor(0xd5d0cf);
		paint.setStyle(Paint.Style.STROKE);
		// 设置画笔笔画宽度
		paint.setAlpha(50);
		paint.setStrokeWidth(4);
		paint.setAntiAlias(true);
		paint.setDither(true);

	}

	private float x = 0, y = 0;
	private boolean isOnePoint;

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		int pointerCount = event.getPointerCount();
		int action = event.getAction();

		if (pointerCount == 1) {
			switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				isOnePoint = true;
				x = event.getX(0);
				y = event.getY(0);
//				Log.v(TAG, "1 points ACTION_DOWN ");
				break;
			case MotionEvent.ACTION_UP:
				break;
			case MotionEvent.ACTION_MOVE:
				if (isOnePoint) {
					originx += (event.getX(0) - x);
					originy += (event.getY(0) - y);
					x = event.getX(0);
					y = event.getY(0);
//					Log.v(TAG, "1 points ACTION_MOVE ");
					invalidate();
				}
				break;
			}
			// 根据位置画圆圈
		} else {
			switch (action) {
			case MotionEvent.ACTION_POINTER_2_DOWN:
				isOnePoint = false;
				x0 = event.getX(0);
				y0 = event.getY(0);
				x1 = event.getX(1);
				y1 = event.getY(1);
				curdistance = (int) Math.sqrt((x1 - x0) * (x1 - x0) + (y1 - y0)
						* (y1 - y0));
//				Log.v(TAG + "2 points ACTION_DOWN ", "curdistance = "
//						+ curdistance + "x1:" + x1 + "y1:" + y1 + "radius = "
//						+ radius);
				break;
			case MotionEvent.ACTION_UP:
				break;
			case MotionEvent.ACTION_MOVE:
				x0 = event.getX(0);
				y0 = event.getY(0);
				x1 = event.getX(1);
				y1 = event.getY(1);
				int distance = (int) Math.sqrt((x1 - x0) * (x1 - x0)
						+ (y1 - y0) * (y1 - y0));
				radius += ((distance - curdistance));
//				Log.v(TAG, "distance = " + distance + "curdistance = "
//						+ curdistance + "radius = " + radius);
				if (radius < 10) {
					radius = 10;
				}
				curdistance = distance;
				invalidate();
				break;
			}
		}
		return true;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		left = originx - radius;
		top = originy - radius;
		right = originx + radius;
		bottom = originy + radius;

		if(rectf == null){
			rectf = new RectF(left, top, right, bottom);
		}
		rectf.set(left, top, right, bottom);
		canvas.drawOval(rectf, paint);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		// widthMeasureSpec、heightMeasureSpec是有高16位模式和低16位size,所以看起来会很大
		// 获得模式
		int mode = MeasureSpec.getMode(widthMeasureSpec);
		// AT_MOST限制最大尺寸,EXACTLY则是确定好尺寸了,例如在xml文件中设置了大小
		if (mode == MeasureSpec.AT_MOST || mode == MeasureSpec.EXACTLY) {
			// 获得View实际大小
			int width = MeasureSpec.getSize(widthMeasureSpec);
			int height = MeasureSpec.getSize(heightMeasureSpec);
			originx = width / 2;
			originy = height / 2;
			// 以下方法是设置View大小的方法,除非想在代码中控制大小,不然不应该在这里填写大小
			// 如果希望其生效则不要调用super.onMeasure(widthMeasureSpec,
			// heightMeasureSpec);这句了
			// setMeasuredDimension(400, height);
		} else if (mode == MeasureSpec.UNSPECIFIED) { // View的大小不确定时
			Log.d("WOGU", "mode=UNSPECIFIED");
		}
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	}
	
	public void setXY(float x,float y){
		originx = x;
		originy = y;
		invalidate();
	}
	
	public void setRadius(int r){
		radius = r;
		invalidate();
	}

	public void setAlpha(int a){
		if(a>=0 && a<=255){
			paint.setAlpha(a);
			invalidate();
		}
	}
	
	public void setStrokeWidth(float w){
		paint.setStrokeWidth(w);
		invalidate();
	}
	
	public float getX(){
		return originx;
	}
	
	public float getY(){
		return originy;
	}
	
	public int getRadius(){
		return radius;
	}
}
