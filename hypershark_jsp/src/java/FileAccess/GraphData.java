/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.Graph.Statistics;
import java.util.ArrayList;

/**
 *
 * @author varun
 */
public class GraphData
{
    int totPoints;
    private ArrayList<Statistics> stats=new ArrayList<Statistics>();
    
    public void addStat(int xValue,int yValue)
    {
        Statistics stat=new Statistics();
        stat.setAbscissa(xValue);
        stat.setOrdinate(yValue);
        stats.add(stat);
    }        
    public native void fillips();//for ips
    public native void fillSrcPorts();//for source port
    
    static
    {
        System.loadLibrary("graph");
    }
    public ArrayList<Statistics> getTopSrcIps(int no)
    {
        totPoints=no;
        fillips();
        return stats;
    }

    public ArrayList<Statistics> getStats() {
        return stats;
    }

    public void setStats(ArrayList<Statistics> stats) {
        this.stats = stats;
    }

    public int getTotPoints() {
        return totPoints;
    }

    public void setTotPoints(int totPoints) {
        this.totPoints = totPoints;
    }
    public ArrayList<Statistics> getTopSrcPorts(int no)
    {
        totPoints=no;
        fillSrcPorts();
        return stats;
        
    }        
    public static void main(String args[])
    {
    
        GraphData graphobj=new GraphData();
        graphobj.totPoints=5;
        //graphobj.fillips();//call native method
        graphobj.fillSrcPorts();
        int i=0;
        System.out.println("count is"+graphobj.totPoints+"  array size is"+graphobj.getStats().size());
        for(Statistics temp : graphobj.getStats())
        {
            System.out.println(temp.getAbscissa()+"   "+temp.getOrdinate());
            i++;
        }
    }
        
}


    
    
    
    