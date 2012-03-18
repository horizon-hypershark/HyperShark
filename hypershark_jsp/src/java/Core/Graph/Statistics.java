/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Graph;

/**
 *
 * @author root
 */
public class Statistics<Type> {
    private Type ordinate;
    private Type abscissa;
    private String info;

    public Type getAbscissa() {
        return abscissa;
    }

    public String getInfo() {
        return info;
    }

    public void setInfo(String info) {
        this.info = info;
    }

    public void setAbscissa(Type abscissa) {
        this.abscissa = abscissa;
    }

    public Type getOrdinate() {
        return ordinate;
    }

    public void setOrdinate(Type ordinate) {
        this.ordinate = ordinate;
    }

    
    
}
