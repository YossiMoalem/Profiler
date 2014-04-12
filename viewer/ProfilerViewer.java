package profilerviewer;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author YOSIM
 */
public class ProfilerViewer {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try {
            new DataFileReader(args[0]).read();
        } catch (IOException ex) {
            Logger.getLogger(ProfilerViewer.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        ProfilerMainView mainFrame = new ProfilerMainView();
        mainFrame.setVisible(true);
    }
    
}
