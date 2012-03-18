/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import Core.Graph.Statistics;
import FileAccess.GraphData;
import java.util.ArrayList;

/**
 *
 * @author root
 */
public class Graph {
    public ArrayList<Statistics> getTopSrcIps(int no)
    {
        GraphData data =new GraphData();
        return data.getTopSrcIps(no);
    }
    public ArrayList<Statistics> getTopSrcPorts(int no)
    {
        GraphData data = new GraphData();
        return data.getTopSrcPorts(no);
    }
}
