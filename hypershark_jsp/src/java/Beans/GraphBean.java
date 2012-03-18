/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Beans;

import Controller.FlowController;
import Core.Graph.Statistics;
import java.util.ArrayList;

/**
 *
 * @author root
 */
public class GraphBean
{
    private String param;
    private String ordinateLabel;
    private String abscissaLabel;
    
    public ArrayList<Statistics> getDataToPlot(int no)
    {
        FlowController controller = new FlowController();
        return controller.getGraphData(param, no,ordinateLabel,abscissaLabel);
    }        
    public String getParam() {
        return param;
    }

    public void setParam(String param) {
        this.param = param;
    }

    public String getAbscissaLabel() {
        return abscissaLabel;
    }

    public void setAbscissaLabel(String abscissaLabel) {
        this.abscissaLabel = abscissaLabel;
    }

    public String getOrdinateLabel() {
        return ordinateLabel;
    }

    public void setOrdinateLabel(String ordinateLabel) {
        this.ordinateLabel = ordinateLabel;
    }
   
}
