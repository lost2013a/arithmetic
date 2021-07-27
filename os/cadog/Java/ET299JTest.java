import ET299jni.*;
import java.lang.*;
import java.io.*;

class ET299JTest
{
	// This routine converts bytes into hexadecimal representation
	  public static void printHexString(String hint, byte[] b, int len) 
	  {        
	  	  System.out.println(hint);
	  	  //
        for (int i = 0; i < len/16; i++) 
        {
        	 for (int j = 0; j < 16; j++)
        	 {
              String hex = Integer.toHexString(b[16 * i + j] & 0xFF);
              if (hex.length() == 1) 
              {
                 hex = '0' + hex;
              }
              System.out.print(hex.toUpperCase() + " ");
              //
              if(j == 7) System.out.print("- ");
              //              
           }           
           System.out.println("");
        }
        System.out.println("");
    }

    public static String Bytes2HexString(byte[] b) 
    {
       String ret = "";
       //
       for (int i = 0; i < b.length; i++) 
       {
          String hex = Integer.toHexString(b[i] & 0xFF);
          if (hex.length() == 1) 
          {
            hex = '0' + hex;
          }
          //
          ret += hex.toUpperCase();
      }
      return ret;
    }
  
    public static byte uniteBytes(byte src0, byte src1) 
    {
       byte _b0 = Byte.decode("0x" + new String(new byte[]{src0})).byteValue();
       _b0 = (byte)(_b0 << 4);
       byte _b1 = Byte.decode("0x" + new String(new byte[]{src1})).byteValue();
       byte ret = (byte)(_b0 ^ _b1);
       return ret;
    }
  
    public static byte[] HexString2Bytes(String src)
    {
       byte[] ret = new byte[8];
       byte[] tmp = src.getBytes();
       //
       for(int i=0; i<8; i++)       
       {
          ret[i] = uniteBytes(tmp[i*2], tmp[i*2+1]);
       }
       return ret;
    }
  
    public static char Hex(int bin) 
    {
        char retval;
        
        if (bin >= 0 && bin <= 9)
            retval = (char)('0' + bin);
        else if (bin >= 10 && bin <= 15)
            retval = (char)('A' + bin - 10);
        else
            retval = '0';
        return retval;
    }
    
    public static int B2Int(byte [] bina) 
    {
        int retval = 0;
        int t1 = 0;
        
        t1 = bina[3];
        retval = t1 & 0xFF;
        t1 = bina[2] & 0xFF;
        retval = retval * 0x100 + t1;
        t1 = bina[1] & 0xFF;
        retval = retval * 0x100 + t1;
        t1 = bina[0] & 0xFF;
        retval = retval * 0x100 + t1;
        return retval;
    }
    
    public static void TSleep()
    {
    	try
    	{
    		Thread.sleep(1000);
    	}
    	catch(InterruptedException e)
    	{
			  System.err.println("Trouble with the lamp flash.");
		  }
    }
    
    public static class SYSTEMTIME implements Serializable 
    {
        short wYear           = 0; 
        short wMonth          = 0; 
        short wDayOfWeek      = 0; 
        short wDay            = 0; 
        short wHour           = 0; 
        short wMinute         = 0; 
        short wSecond         = 0; 
        short wMilliseconds   = 0; 
    }    
    
    public static void swap(byte[] bytes)
    {
    	  byte a;
    	  //
        for(int i=0; i<bytes.length; i+=2)
        {
        	 a = bytes[i];
        	 bytes[i] = bytes[i+1];
        	 bytes[i+1] = a;
        }
    }
    
    //
    public static void ST2BYTE(byte[] byte_out, SYSTEMTIME st)
    {
    	  try
    	  {
    	     ByteArrayOutputStream  baos = new ByteArrayOutputStream(); 
    	     DataOutputStream dos = new DataOutputStream(baos);   
    	     //
    	     dos.writeShort(st.wYear);
           dos.writeShort(st.wMonth);
           dos.writeShort(st.wDayOfWeek);
           dos.writeShort(st.wDay);
           dos.writeShort(st.wHour);
           dos.writeShort(st.wMinute);
           dos.writeShort(st.wSecond);
           dos.writeShort(st.wMilliseconds);
           //dos.flush();
           byte[] pb = baos.toByteArray(); 
           //
           for(int i=0; i<baos.size(); i++)
           {
        	    byte_out[i] = pb[i];        	    
           }
           //
           swap(byte_out);
           //
           baos.close(); 
           dos.close(); 
        }
        catch(Exception e)
        { 
           System.out.println("translation"+e.getMessage()); 
           //e.printStackTrace();             
        } 
    }
    
    public static void BYTE2ST(byte[] byte_in, SYSTEMTIME st)
    {
    	  try
    	  {    	  	 
    	     swap(byte_in); 	
    	     //
    	     ByteArrayInputStream bais = new ByteArrayInputStream(byte_in);
    	     DataInputStream dis = new DataInputStream(bais);       	     
    	     //
    	     st.wYear         = dis.readShort();
    	     st.wMonth        = dis.readShort();
    	     st.wDayOfWeek    = dis.readShort();
    	     st.wDay          = dis.readShort();
    	     st.wHour         = dis.readShort();
    	     st.wMinute       = dis.readShort();
    	     st.wSecond       = dis.readShort();
    	     st.wMilliseconds = dis.readShort();
    	     //
    	     bais.close(); 
           dis.close(); 
       }
       catch(Exception e)
       { 
           System.out.println("translation"+e.getMessage()); 
           //e.printStackTrace();             
       }   	    	
      
    }
    
    /*
    public static void Serialize(DataOutputStream byte_out, SYSTEMTIME st) throws IOException 
    {
    	  byte_out.writeShort(st.wYear);
        byte_out.writeShort(st.wMonth);
        byte_out.writeShort(st.wDayOfWeek);
        byte_out.writeShort(st.wDay);
        byte_out.writeShort(st.wHour);
        byte_out.writeShort(st.wMinute);
        byte_out.writeShort(st.wSecond);
        byte_out.writeShort(st.wMilliseconds);
        byte_out.flush();
    }
    
    
    //
    public static SYSTEMTIME UnSerialize(DataInputStream byte_in) throws IOException
    {
    	SYSTEMTIME st = new SYSTEMTIME();
    	//
    	st.wYear         = byte_in.readShort();
    	st.wMonth        = byte_in.readShort();
    	st.wDayOfWeek    = byte_in.readShort();
    	st.wDay          = byte_in.readShort();
    	st.wHour         = byte_in.readShort();
    	st.wMinute       = byte_in.readShort();
    	st.wSecond       = byte_in.readShort();
    	st.wMilliseconds = byte_in.readShort();
    	    	
      return st;
    }
   
    public static java.lang.Object ByteToObject(byte[] bytes)
    { 
        java.lang.Object obj; 
        //
        try
        { 
           //bytearray to object 
           ByteArrayInputStream bi = new ByteArrayInputStream(bytes); 
           ObjectInputStream    oi = new ObjectInputStream(bi); 
           //
           obj = oi.readObject(); 
           //
           bi.close(); 
           oi.close();             
        } 
        catch(Exception e)
        { 
            System.out.println("translation"+e.getMessage()); 
            //e.printStackTrace(); 
            obj = null;
        } 
        //
        return obj;        
    } 
    
    public static byte[] ObjectToByte(java.lang.Object obj) 
    { 
        byte[] bytes; 
        //
        try
        { 
            //object to bytearray 
            ByteArrayOutputStream bo = new ByteArrayOutputStream(); 
            ObjectOutputStream    oo = new ObjectOutputStream(bo); 
            //
            oo.writeObject(obj); 
            //
            bytes = bo.toByteArray(); 
            //
            bo.close(); 
            oo.close();                 
        } 
        catch(Exception e)
        { 
            System.out.println("translation"+e.getMessage()); 
            //e.printStackTrace(); 
            bytes = null;
        } 
        //
        return bytes;         
    } 
    */
    
    
	public static void main(String[] args)
	{
		IET299 ET299 = new CET299();
		ET299Def flag = new ET299Def();
		SYSTEMTIME st = new SYSTEMTIME();
		byte[] tText = new byte[64];
		String chPid = "";
		String chPin = "";
		String chSoPin = "";
		String Seed = "1234567890";
		boolean DigestFlag = true;
		
		byte[] TokenKey = new byte[32];
		byte[] TokenDigest = new byte[16];
		byte[] SoftDigest = new byte[16];
		byte[] bSeed = new byte[10];
		byte[] ran = new byte[16];
		byte[] SN = new byte[8];
		int[] tokenCount = new int[1];
		byte[] pid = new byte[8];
		byte[] userPin = new byte[16];
		byte[] soPin = new byte[16];
		char[] charPid = new char[8];
		char[] charPin = new char[16];
		char[] charSoPin = new char[16];
		char[] newPid = new char[8];
		char[] newSoPin = new char[16];
		
		int[]  leng   = new int[1];
		byte[] pubkey = new byte[128];
		byte[] prikey = new byte[128];
		byte[] tmpbuf = new byte[128];
				
		
		for(int j=0;j<10;j++)
		{
			bSeed[j] = (byte)Seed.charAt(j);
		}
		
		BufferedReader lineOfText = new BufferedReader(new InputStreamReader(System.in));
		try
		{
			System.out.println("Please input PID of the first ET299 (8 char) : ");
			chPid = lineOfText.readLine();
			for(int i=0;i<8;i++)
			{
				charPid[i] = chPid.charAt(i);
				pid[i] = (byte)charPid[i];
			}
			System.out.println("Please input User PIN of the first ET299 (16 char) : ");
			chPin = lineOfText.readLine();
			for(int j=0;j<16;j++)
			{
				charPin[j] = chPin.charAt(j);
				userPin[j] = (byte)charPin[j];
			}
			System.out.println("Please input SO PIN of the first ET299 (16 char) : ");
			chSoPin = lineOfText.readLine();
			for(int j=0;j<16;j++)
			{
				charSoPin[j] = chSoPin.charAt(j);
				soPin[j] = (byte)charSoPin[j];
			}
		}
		catch (IOException e)
		{
			System.err.println("Trouble with the input pid.");
		}
		
		try
		{
			System.out.println("Try to Find ET299...");
			ET299.FindToken(pid,tokenCount);
			if (tokenCount[0]!=0)
				System.out.println("Find ET299!");
			System.out.println("Try to open a token...");
			ET299.OpenToken(pid,1);
			System.out.println("Open a ET299 success!");
			System.out.println("Verify user pin...");
			ET299.Verify(0,userPin);
			System.out.println("Verify user pin success!");    
			System.out.println("Write ET299...");         
            ET299.Write(0, 16, userPin);
            System.out.println("Write succeed!" );
            System.out.println("Read ET299...");     
            ET299.Read(0, 16, tText);
			System.out.println("Read succeed! ");
			System.out.println("Get random...");            
            ET299.GenRandom(ran);
            System.out.println("Get random success!");
            System.out.println("Get SN...");            
            ET299.GetSN(SN);
            System.out.println("Get SN success!");
            ET299.Verify(1,soPin);
            System.out.println("SoftMD5HMAC generate token key...");
            ET299.SoftMD5HMAC(bSeed,10,bSeed,10,TokenKey,SoftDigest);
            System.out.println("SoftMD5HMAC generate token key success!");
            System.out.println("Set Token Key...");
            ET299.SetKey(1,TokenKey);
            System.out.println("Set Token Key success!");
            System.out.println("SoftMD5HMAC generate soft digest...");
            ET299.SoftMD5HMAC(bSeed,10,bSeed,10,TokenKey,SoftDigest);
            System.out.println("SoftMD5HMAC generate soft digest success!");
            System.out.println("MD5HMAC generate Token digest...");
            ET299.MD5HMAC(1,bSeed,10,TokenDigest);
            System.out.println("MD5HMAC generate Token digest success!");
            System.out.println("Compare Soft digest and Token digest...");
            for(int k=0;k<16;k++)
            {
            	if(TokenDigest[k]!=SoftDigest[k])
            	{
            		DigestFlag = false;
            		System.out.println("Compare Digest fail!");
            		break;
            	}
            }
            if(DigestFlag==true)
            	System.out.println("Compare Digest success!");
            System.out.println("Change User Pin...");
            ET299.ChangeUserPIN(userPin,userPin);
            System.out.println("Change User Pin success!");
            System.out.println("The lamp is flashing...");
            for(int j=0;j<1;j++)
            {
            	ET299.TurnOffLED();
            	TSleep();
            	ET299.TurnOnLED();
            	TSleep();       	
            }	
            System.out.println("The lamp is flashing finish.");
            System.out.println("Verify So pin...");
			ET299.Verify(1,soPin);
			System.out.println("Verify So pin success!");  
			System.out.println("SetupToken...");
			ET299.SetupToken(0,0,0,0);
			System.out.println("SetupToken success!");  
			//if you want to run follows code, please record the PID and sopin
					//System.out.println("GenPid...");
					//ET299.GenPid(bSeed,10,pid);
					//System.out.println("GenPid success!");  
					//System.out.println("Gen So PIN ...");
					//ET299.GenSoPIN(bSeed,10,soPin);
					//System.out.println("Gen So PIN success!");
			System.out.println("ReSetPin ...");
			ET299.ResetPIN(soPin);
			System.out.println("ResetPin success!");
			System.out.println("Token user pin is FFFFFFFFFFFFFFFF");
			
			//==============================================================================
			for(int i=0; i<32; i++)
			{
			   TokenKey[i] = (byte)0xFF; 
		  }
			System.out.println("SetKeyEx ...");
			ET299.SetKeyEx(0, 1, TokenKey); //ET_KEY_HMAC_MD5
			System.out.println("SetKeyEx success!");	
			//
			for(int i=0; i<32; i++)
			{
			   TokenKey[i] = (byte)0x00; 
		  }
		  for(int i=0; i<8; i++)
		  {
		  	 TokenKey[i] = (byte)(0x31+i); 
		  }
			System.out.println("SetSNEx(12345678) ......");	
			ET299.SetSNEx(TokenKey, 8);
			System.out.println("SetSNEx success!");		
			//
			for(int i=0; i<64; i++)
			{
			   tText[i] = (byte)0x00;  
		  }
			System.out.println("GetSNEx() ......");
			leng[0] = 64;
			ET299.GetSNEx(tText, leng);
			System.out.println(new String(tText));
			System.out.println("GetSNEx success!");		
			//
			System.out.println("RSAGenKey() ......");
			ET299.RSAGenKey(1024, pubkey, prikey);
			printHexString("PublicKey:",pubkey, 128);
			printHexString("PrivateKey:",prikey, 128);
			//
			System.out.println("RSASetKey() ......");
			ET299.RSASetKey(1024, pubkey, prikey);
			System.out.println("RSASetKey success!");	
			//
			for(int i=0; i<128; i++)
			{
			   tmpbuf[i] = (byte)i;  
		  }
		  leng[0] = 128;
			System.out.println("RSAPublicEncrypt() ......");
			printHexString("Original Data:",tmpbuf, 128);
			ET299.RSAPublicEncrypt(tmpbuf, leng, 2); 
			printHexString("RSAPublicEncrypt:",tmpbuf, 128);
			System.out.println("RSAPublicEncrypt success!");	
			//
			System.out.println("RSAPrivateDecrypt() ......");
			ET299.RSAPrivateDecrypt(tmpbuf, leng, 2); 
			printHexString("RSAPrivateDecrypt:",tmpbuf, 128);
			System.out.println("RSAPrivateDecrypt success!");	
			//
			System.out.println("RSAPrivateEncrypt() ......");
			ET299.RSAPrivateEncrypt(tmpbuf, leng, 2); 
			printHexString("RSAPrivateEncrypt:",tmpbuf, 128);
			System.out.println("RSAPrivateEncrypt success!");	
			//
			System.out.println("RSAPublicDecrypt() ......");
			ET299.RSAPublicDecrypt(tmpbuf, leng, 2); 
			printHexString("RSAPublicDecrypt:",tmpbuf, 128);
			System.out.println("RSAPublicDecrypt success!");	
			//
			System.out.println("TDesEncrypt() ......");
			ET299.TDesEncrypt(1, tmpbuf, 128); 
			printHexString("TDesEncrypt:",tmpbuf, 128);
			System.out.println("TDesEncrypt success!");	
			//
			System.out.println("TDesDecrypt() ......");
			ET299.TDesDecrypt(1, tmpbuf, 128); 
			printHexString("TDesDecrypt:",tmpbuf, 128);
			System.out.println("TDesDecrypt success!");	
			//Set Time License
			System.out.println("SetTimeLicense(2020-01-02 12:16:18) ......");
			for(int i=0; i<128; i++)
			{
			   tmpbuf[i] = (byte)0x00;   
		  }
		  st.wYear   = 2020;
	    st.wMonth  = 1;
	    st.wDay    = 2;
	    st.wHour   = 12;
	    st.wMinute = 16;
      st.wSecond = 18;
	    st.wMilliseconds = 0;
	    st.wDayOfWeek    = 0;	    
	    ST2BYTE(tmpbuf, st);
	    printHexString("st:",tmpbuf, 16);
		  ET299.SetTimeLicense(0, 0, tmpbuf); //ET_EXPIRE_DATE
		  System.out.println("SetTimeLicense success!");
			//Get Time License
			System.out.println("GetTimeLicense() ......");
			for(int i=0; i<128; i++)
			{
			   tmpbuf[i] = (byte)0x00;   
		  }
		  leng[0] = -1;
		  ET299.GetTimeLicense(0, leng, tmpbuf);
		  BYTE2ST(tmpbuf, st);		  
		  System.out.println(st.wYear + "-" + st.wMonth + "-" + st.wDay + " " + st.wHour + ":" + st.wMinute + ":" + st.wSecond);
		  System.out.println("GetTimeLicense success!");
			//Clear Time License
			System.out.println("Clear Time License() ......");
			for(int i=0; i<128; i++)
			{
			   tmpbuf[i] = (byte)0xFF;   
		  }
		  ET299.SetTimeLicense(0, 0, tmpbuf); //ET_EXPIRE_DATE
		  System.out.println("Clear Time License success!");
		  //=========================
		  System.out.println("SetTimeLicense(120 minutes) ......");
			for(int i=0; i<128; i++)
			{
			   tmpbuf[i] = (byte)0x00;   
		  }
		  tmpbuf[0] = (byte)0x78; //120;
		  ET299.SetTimeLicense(0, 1, tmpbuf); //ET_TIME_MINUTE
		  System.out.println("SetTimeLicense success!");
		  //
		  System.out.println("StartTimeUnit(0) ......");
		  ET299.StartTimeUnit(0);	
		  System.out.println("StartTimeUnit(0) success!");
		  //
		  System.out.println("StopTimeUnit(0) ......");
		  ET299.StopTimeUnit(0);	
		  System.out.println("StopTimeUnit(0) success!");
		  //
		  System.out.println("Clear Time License() ......");
			for(int i=0; i<128; i++)
			{
			   tmpbuf[i] = (byte)0xFF;   
		  }
		  ET299.SetTimeLicense(0, 1, tmpbuf); //ET_TIME_MINUTE
		  System.out.println("Clear Time License success!");
		  //=========================
		  System.out.println("SetCountLicense(100, enable_dec) ......");
		  ET299.SetCountLicense(0, (int)100, (byte)1);  //ET_ENABLE_DECREASE = 1
		  System.out.println("SetCountLicense success!");
		  //		 
		  int[] cout = new int[1];
		  byte[] typa = new byte[1]; 		  
		  System.out.println("GetCountLicense() ......");		  
		  ET299.GetCountLicense(0, cout, typa);		 		  
		  System.out.println("count:" + cout[0] + "    type:" + typa[0]);
		  System.out.println("GetCountLicense success!");
		  //
		  System.out.println("GetCountLicense() ......");
		  ET299.GetCountLicense(0, cout, typa);
		  System.out.println("count:" + cout[0] + "    type:" + typa[0]);
		  System.out.println("GetCountLicense success!");
		  //
		  System.out.println("Clear Count License ......");
		  ET299.SetCountLicense(0, (int)-1, (byte)0);  //ET_DISABLE_DECREASE = 0
		  System.out.println("Clear Count License success!");
		  //
		  short[] ver = new short[1];
		  System.out.println("GetVersion() ......");
		  ET299.GetVersion(ver);
		  System.out.println(ver[0]);
		  System.out.println("GetVersion success!");
			//==============================================================================
			
      System.out.println("ResetSecurityState...");            
      ET299.ResetSecurityState();
      System.out.println("ResetSecurityState success!");
						
			System.out.println("Close the opened token...");
			ET299.CloseToken();
			System.out.println("Close the opened token success!");
		}
		catch(RTException e)
		{
			System.out.println(e.HResult()+":"+e.getMessage());
			ET299.CloseToken();
		}
	}
}
