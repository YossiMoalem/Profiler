package profilerviewer;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author YOSIM
 */
public class DataFileReader {

    public DataFileReader(final String iFilePath) 
    {
        mFilePath = iFilePath;
    }
    
    public int read() throws IOException
    {
        Path filePath = Paths.get(mFilePath);
        //TODO: check that file exists...
        List<String> data = Files.readAllLines(filePath, Charset.defaultCharset());
        final String dictionaryString = "^\\s*([0-9a-fx]+)\\s*=\\s*(\\S+)\\s*$";
        final Pattern dictionaryPattern = Pattern.compile(dictionaryString);
              
        final String stackString = "^\\s*([0-9a-fx: ]+)=(\\d+)\\s*$";
        final Pattern stackPattern = Pattern.compile(stackString);
        
        //TODO: verify order...
        for (String curLine : data)
        {
            final Matcher dictionaryMatcher = dictionaryPattern.matcher(curLine);
            if (dictionaryMatcher.find())
            {
                addNewFrame(dictionaryMatcher.group(1), dictionaryMatcher.group(2));
            }
            final Matcher stackMatcher = stackPattern.matcher(curLine);
            if (stackMatcher.find())
            {
                addNewStack (stackMatcher.group(1), Integer.parseInt(stackMatcher.group(2)));
            } 
            
        }
        return 0;
    }

    private void addNewFrame(String iAddress, String iName) 
    {
          Frame newFrame = new Frame(iAddress, iName);
          FrameDictionary.instance().addFrame(newFrame);
    }

    private void addNewStack(String iStack, int hits) 
    {
        Stack curStack = new Stack(iStack, hits);
        boolean isLeaf = true;
        for (Frame curFrame : curStack.getStackFrames())
        {
            if (isLeaf == true)
            {
                isLeaf = false;
                curFrame.addLeafStack(curStack);
            } else {
                curFrame.addParticipantStack(curStack);
            } 
        }  
    }
    
    final private String mFilePath;
}
