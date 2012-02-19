/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

function verify(input)
{
    var ret;
    ret=verify_mac(document.getElementById(input).value);
    switch(ret)
    {
        case 0:
            alert("insufficient address length: example address AB:01:23:CD:EF:45");
            break;
        case 2:
            alert(" : missing ");
            break;
        case 3:
            alert("invalid characters : only 0-9 and A-F acceptable ");
            break;
    }
}

function verify_mac(add)
{
    var i,ch;


    if(add.length!=17)
        return(0);
    for(i=0;i<add.length;i++)
    {
        ch=add.charCodeAt(i);
        if(i==2||i==5||i==8||i==11||i==14)
        {
            if(ch!=':')
                return(2);    
        }
        else
        {
            if(!((ch>47&&ch<59)||(ch>96&&ch<103)||(ch>64&&ch<71)))
            {
                return(3);
            }    
        }    
    }
    return(1);
}




<script type="text/javascript">

function verify_port(port)
{
    var ret;
    ret=checkport(document.getElementById(port).value);
    switch(ret)
    {
        case 0:
            alert("Port address empty");
            break;
        case 2:
            alert("value out of range");
            break;
        case 3:
            alert("Value invalid, Only digits allowed ");
            break;
        default:
            return;

    }
}    




function checkport(input)
{
    var i,input;


    if(input=="")
    {
        return(0);
    }
    if(input>65535)
    {
        return(2);
    }
    else
    {
        for(i=0;i<input.length;i++)
        {
            ch=input.charCodeAt(i);
            if(!(ch>47&&ch<59))
            {
                return(3);//different return codes based on error 
            }    
        }    
    }

    return(1);

}
                                                                        
                                                                                   

