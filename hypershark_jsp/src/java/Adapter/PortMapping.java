/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import FileAccess.PortToNumberMapping;

/**
 *
 * @author root
 */
public class PortMapping
{
    public String getPortByNumber(int no)
    {
        return PortToNumberMapping.getPortByNumber(no);
    }
    public int getPortByName(String name)
    {
        return PortToNumberMapping.getPortByName(name);
    }
}
