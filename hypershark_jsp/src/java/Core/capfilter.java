/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

/**
 *
 * @author varun
 */
public class capfilter
{

        public CaptureRule cpt=new CaptureRule();
        public native void fillcapfilter();
         
        static {
            System.loadLibrary("capfilter");
        }
        public static void main(String args[])
        {
            capfilter filter=new capfilter();
            short[] sourcemac=new short[6];
            short[] destmac=new short[6];
            short[] sourceip=new short[4];
            short[] destip=new short[4];
            
            int i;
            
            for(i=0;i<6;i++)
            {
                sourcemac[i]=(short) (10+i);
            }
            for(i=0;i<6;i++)
            {
                destmac[i]=(short) (20+i);
            }
            
            filter.cpt.setSmac(sourcemac);
            filter.cpt.setDmac(destmac);
            
            for(i=0;i<4;i++)
            {
                sourceip[i]=(short) (100+i);
            }
            for(i=0;i<4;i++)
            {
                destip[i]=(short) (120+i);
            }
            
            filter.cpt.setDstHost(destip);
            filter.cpt.setSrcHost(sourceip);
            
            
            filter.cpt.setHighSrcPort(150);
            filter.cpt.setLowSrcPort(50);
            filter.cpt.setProtocol((short)20);
            
            filter.fillcapfilter();    
            
         
        }

}
