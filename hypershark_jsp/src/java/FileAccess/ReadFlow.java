/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.FlowRecord;
import Utils.Conversions;

/**
 *
 * @author varun
 */
public class ReadFlow 
{
        public FlowRecord flow[]=new FlowRecord[100];
        public ReadFlow()
                {
                    for(int i=0;i<100;i++)
                    {
                       flow[i]=new FlowRecord(); 
                    }
                }
	       
        public native void readflow();
	static
	{
		System.loadLibrary("flowrecord");
	}
	public static void main(String args[])
	{
           
         /*int j=0,i=0;
         ReadFlow flow_obj=new ReadFlow(); 
         
         flow_obj.readflow();
         while(j<10)
          {
             System.out.println(Conversions.shortToUnsigned(flow_obj.flow[j].src_port));
             System.out.println(Conversions.shortToUnsigned(flow_obj.flow[j].dst_port));
             System.out.println(((flow_obj.flow[j].ip_src) & 0xFF) + "." + ((flow_obj.flow[j].ip_src >> 8) & 0xFF) + "." + ((flow_obj.flow[j].ip_src>> 16) & 0xFF) +"."+ ((flow_obj.flow[j].ip_src>> 24) & 0xFF));
             System.out.println(((flow_obj.flow[j].ip_dst) & 0xFF) + "." + ((flow_obj.flow[j].ip_dst >> 8) & 0xFF) + "." + ((flow_obj.flow[j].ip_dst>> 16) & 0xFF) +"."+ ((flow_obj.flow[j].ip_dst>> 24) & 0xFF));
             System.out.println(flow_obj.flow[j].protocol);
             System.out.println(flow_obj.flow[j].nop);
             j++;
         }*/
       }
}
