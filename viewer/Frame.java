package profilerviewer;

import java.util.ArrayList;

/**
 *
 * @author YOSIM
 */
public class Frame {
    public Frame (final String iAddress, final String iName)
    {
        mAddress = iAddress;
        mName = iName;
    }
    
    public int getNumOfLeafStacks ()
    {
        return mLeafStack.size();
    }
    
    public int getNumOfParticipatingStack ()
    {
        return mParticipateStack.size();
    }
    
    public void addLeafStack (final Stack iNewStrack)
    {
        mLeafStack.add(iNewStrack);
        mLeafHits += iNewStrack.getHits();
    }
    
    public void addParticipantStack (final Stack iNewStack)
    {
        mParticipateStack.add(iNewStack);
        mParticipateHits += iNewStack.getHits();
    }
    
    public final String getName ()
    {
        return mName;
    }
    
    public final String getAddress ()
    {
        return mAddress;
    }
    
    public ArrayList<Stack> getLeafStack ()
    {
        return mLeafStack;
    }
    
    public ArrayList<Stack> getParticipantStack ()
    {
        return mParticipateStack;
    }
    
    public int getNumOfLeafHits ()
    {
        return mLeafHits;
    }
    
    public int getnumOfParticipantHits ()
    {
        return mParticipateHits;
    }
    private final String mAddress;
    private final String mName;
    private final ArrayList<Stack>  mLeafStack = new ArrayList<>();
    private final ArrayList<Stack>  mParticipateStack = new ArrayList<>();
    int mLeafHits = 0;
    int mParticipateHits = 0;
}
