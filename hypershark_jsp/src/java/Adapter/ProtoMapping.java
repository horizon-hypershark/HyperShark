/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import FileAccess.L3ProtocolMapping;

/**
 *
 * @author root
 */
public class ProtoMapping
{
    public String getProtoByNumber(int no)
    {
        return L3ProtocolMapping.getProtoByNumber(no);
    }
    
}
    

