/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package util;

/**
 *
 * @author pankaj
 */
import java.io.*;
import java.util.*;

public class ReadProperty{

  private Properties pro;
  private static ReadProperty props;

  public static ReadProperty getInstance()
    {
        if(props==null)
            props = new ReadProperty();
        return props;
    }

//  public static void main(String[] args) {
//    ReadProperty r =ReadProperty.getInstance();
//    System.out.println(ReadProperty.getInstance().getProperty("sermon.db.url"));
//    System.out.println(r.getProperty("sermon.db.url"));
//  }
  private ReadProperty(){
      try{
      
      //File f = new File("/innovation_home/users/innovation/pict/sermon/props/sermon.properties");
      File f = new File("hypershark.properties");
      if(f.exists()){
      pro = new Properties();
      FileInputStream in = new FileInputStream(f);
      pro.load(in);
      }
      else{
      //System.out.println(System.getProperty("user.dir"));
      System.out.println("File not found!....");
      }
      }
      catch(IOException e){
      System.out.println(e.getMessage());
      }
  }
  public String getProperty(String key)
  {
    return pro.getProperty(key);
  }
}
