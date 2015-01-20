package com.example.photoprocessing.util;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

import com.example.photoprocessing.value.FinalValue;


/**
 * @author xuan
 * 2014-10-30
 * 
 */
public class FileUtil {
	public static void saveInfo(String info){
		File file = new File(FinalValue.BLUZ_SAVE_FOLDER);
		if(!file.exists())
			file.mkdirs();
		File saveFile = new File(FinalValue.BLUZ_SAVE_FOLDER+ "/" + "info.txt");
		FileOutputStream fout = null;
		OutputStreamWriter osw = null;
		 try {
			fout = new FileOutputStream(saveFile,true);
			osw =new OutputStreamWriter(fout,"UTF-8");
			osw.write(info);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				osw.close();
				fout.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
