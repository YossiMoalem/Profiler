package profilerviewer;

import java.util.ArrayList;

/**
 *
 * @author YOSIM
 */
public class Stack {
    Stack (final String iStack, int iHits)
    {
        mHits = iHits;
        String[] stackFrames = iStack.split(mFrameDelemeter);
        for (String frameAddress : stackFrames)
        {
            if (frameAddress.compareTo("0") != 0)
            {
                Frame curFrame = FrameDictionary.instance().getFrame(frameAddress);
                mStack.add(curFrame);
            }
        }
    }
    
    final ArrayList<Frame> getStackFrames ()
    {
        return mStack;
    }
    
    int getHits ()
    { 
        return mHits;
    }
    
   final private ArrayList<Frame>     mStack = new ArrayList<>();
   final private int                  mHits;
   private static final String  mFrameDelemeter = ":";
    
}
