
package profilerviewer;

import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * @author YOSIM
 */
public class FrameDictionary {
    
    static FrameDictionary instance ()
    {
        if (sInst == null)
        {
            sInst = new FrameDictionary();
        }
        return sInst;
    }
    
    public int addFrame (final Frame iFrame)
    {
        if (mAdressToFrame.get(iFrame.getAddress()) != null)
        {
            return -1;
        }
        mAdressToFrame.put(iFrame.getAddress(), iFrame);
        mAddressVec.add(iFrame.getAddress());
        return 0;
    }
    
    public Frame getFrame (final String iAddress)
    {
        return mAdressToFrame.get(iAddress);
    }
    
    int getMunOfFrames ()
    {
        return mAdressToFrame.size();
    }
    
    public Frame getFrameAt (int ind)
    {
        return mAdressToFrame.get (mAddressVec.get(ind));
    }
    
    private FrameDictionary() {}
    private final HashMap<String, Frame> mAdressToFrame = new HashMap<>();
    private final ArrayList<String> mAddressVec = new ArrayList<>();
    static private FrameDictionary sInst;
}
