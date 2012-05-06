/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package GeoIP;

import com.maxmind.geoip.*;
import java.io.IOException;
/**
 *
 * @author root
 */
public class GeoInfoLookup {
    public static Location getCity(String ipAddr) {       
	try {
	    LookupService cl = new LookupService("GeoIPCity.dat",
					LookupService.GEOIP_MEMORY_CACHE );
            Location l2 = cl.getLocation(ipAddr);
	    /*System.out.println("countryCode: " + l2.countryCode +
                               "\n countryName: " + l2.countryName +
                               "\n region: " + l2.region +
                               "\n regionName: " + regionName.regionNameByCode(l2.countryCode, l2.region) +
                               "\n city: " + l2.city +
                               "\n postalCode: " + l2.postalCode +
                               "\n latitude: " + l2.latitude +
                               "\n longitude: " + l2.longitude +
 			       "\n metro code: " + l2.metro_code +
 			       "\n area code: " + l2.area_code +
                               "\n timezone: " + timeZone.timeZoneByCountryAndRegion(l2.countryCode, l2.region));
*/
	    cl.close();
            return l2;
	}
	catch (IOException e) {
	    System.out.println("IO Exception");
	}
        return null;
    }
    public static String getOrg(String ipAddr)            
    {        
        String org=null;
        try {
	    LookupService orgl = new LookupService("GeoIPOrg.dat");	
            org=orgl.getOrg(ipAddr);
	    System.out.println("Organization: " + org);			       
	    orgl.close();
	}
	catch (IOException e) {
	    System.out.println("IO Exception");
	}
        return org;
    }        
    public static void main(String args[])
    {
        System.out.println("Organization is :"+getOrg("59.97.193.27"));
    }        
}
