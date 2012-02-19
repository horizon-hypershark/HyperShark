/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Utils;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.*;

import java.util.logging.Level;
import java.util.logging.Logger;
import util.ReadProperty;

/**
 *
 * @author varun
 */
public class Test {
   
   public static void main(String args[])
   {
         
          String url = ReadProperty.getInstance().getProperty("hypershark.db.url");
          System.out.println(url);
        } 
}
