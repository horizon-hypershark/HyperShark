/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

/**
 *
 * @author varun
 */
public class CaptureTime 
{
        public int month;
	public int day;

    public int getDay() {
        return day;
    }

    public void setDay(int day) {
        this.day = day;
    }

    public int getEnd_hr() {
        return end_hr;
    }

    public void setEnd_hr(int end_hr) {
        this.end_hr = end_hr;
    }

    public int getEnd_min() {
        return end_min;
    }

    public void setEnd_min(int end_min) {
        this.end_min = end_min;
    }

    public int getMonth() {
        return month;
    }

    public void setMonth(int month) {
        this.month = month;
    }

    public int getStart_hr() {
        return start_hr;
    }

    public void setStart_hr(int start_hr) {
        this.start_hr = start_hr;
    }

    public int getStart_min() {
        return start_min;
    }

    public void setStart_min(int start_min) {
        this.start_min = start_min;
    }
	public int start_hr,start_min;
	public int end_hr,end_min;
}
