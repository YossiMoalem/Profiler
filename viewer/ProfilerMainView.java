package profilerviewer;

import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.AbstractTableModel;

/**
 *
 * @author YOSIM
 */
public class ProfilerMainView extends javax.swing.JFrame {

    /**
     * Creates new form ProfilerMainView
     */
    public ProfilerMainView() {
        initComponents();
        
        tblMainTable.setModel(new MainTblModel());
        tblMainTable.removeColumn(tblMainTable.getColumnModel().getColumn(MainTblModel.ColumnIndex.Address_INV.ordinal()));
        tblMainTable.setAutoCreateRowSorter(true);
        tblMainTable.getSelectionModel().addListSelectionListener(new ProfilerMainTblSelectionListenerImpl());
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        spMainTable = new javax.swing.JScrollPane();
        tblMainTable = new javax.swing.JTable();
        pnStack = new javax.swing.JPanel();
        paStack = new javax.swing.JScrollPane();
        taStack = new javax.swing.JTextPane();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        getContentPane().setLayout(new java.awt.GridLayout(1, 2));

        tblMainTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Frame", "Hits", "Accum. Hits"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        tblMainTable.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);
        spMainTable.setViewportView(tblMainTable);
        tblMainTable.getColumnModel().getSelectionModel().setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);

        getContentPane().add(spMainTable);

        taStack.setContentType("text/html"); // NOI18N
        paStack.setViewportView(taStack);

        javax.swing.GroupLayout pnStackLayout = new javax.swing.GroupLayout(pnStack);
        pnStack.setLayout(pnStackLayout);
        pnStackLayout.setHorizontalGroup(
            pnStackLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(paStack, javax.swing.GroupLayout.DEFAULT_SIZE, 200, Short.MAX_VALUE)
        );
        pnStackLayout.setVerticalGroup(
            pnStackLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(paStack, javax.swing.GroupLayout.DEFAULT_SIZE, 318, Short.MAX_VALUE)
        );

        getContentPane().add(pnStack);

        pack();
    }// </editor-fold>//GEN-END:initComponents

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(ProfilerMainView.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(ProfilerMainView.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(ProfilerMainView.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(ProfilerMainView.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new ProfilerMainView().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JScrollPane paStack;
    private javax.swing.JPanel pnStack;
    private javax.swing.JScrollPane spMainTable;
    private javax.swing.JTextPane taStack;
    private javax.swing.JTable tblMainTable;
    // End of variables declaration//GEN-END:variables

    private static class MainTblModel extends AbstractTableModel {

        public enum ColumnIndex {
            FrameName,
            LeafCountColumn,
            ParticipantsHiteColumn,
            Address_INV,
            NumOfColumns
        };
        public MainTblModel() 
        {
        }

        @Override
        public int getRowCount() 
        {
            return FrameDictionary.instance().getMunOfFrames();
        }

        @Override
        public int getColumnCount() 
        {
               return (int)ColumnIndex.NumOfColumns.ordinal();
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) 
        {
            Frame f = FrameDictionary.instance().getFrameAt(rowIndex);
            ColumnIndex index = ColumnIndex.values()[columnIndex];
            switch (index)
            {
                case FrameName:
                    return f.getName();
                case LeafCountColumn:
                    return f.getNumOfLeafHits();
                case ParticipantsHiteColumn:
                    return f.getnumOfParticipantHits();
                case Address_INV:
                    return f.getAddress();
                default:
                    assert (false);
            }
            return null;
        }
        
        @Override
        public String getColumnName(int column)
        {
            ColumnIndex index = ColumnIndex.values()[column];
            switch (index)
            {
                case FrameName:
                    return "Frame";
                case LeafCountColumn:
                    return "Hits";
                case ParticipantsHiteColumn:
                    return "Accum. Hits";
                default:
                    assert (false);   
            }
            return null;
        }
        
        @Override
       public Class<?> getColumnClass(int column) 
       {
           if (column == ColumnIndex.LeafCountColumn.ordinal() || 
                   column == ColumnIndex.ParticipantsHiteColumn.ordinal()) 
           {
               return Integer.class;
           }
           return super.getColumnClass(column);
       }
       
       @Override 
       public boolean isCellEditable (int row, int col)
       {
         return false;  
       }
    }

    private class ProfilerMainTblSelectionListenerImpl implements ListSelectionListener {

        public ProfilerMainTblSelectionListenerImpl() {
        }

        @Override
        public void valueChanged(ListSelectionEvent e)
        {
            int selectedLine = tblMainTable.getSelectedRow();
            if (selectedLine >= 0 )
            {
                mSelectedAddress = tblMainTable.getModel().getValueAt(selectedLine, MainTblModel.ColumnIndex.Address_INV.ordinal()).toString();
                Frame selectedFrame = FrameDictionary.instance().getFrame(mSelectedAddress);

                mStackText.setLength(0);
                mStackText.append("<HTML>");
                if (selectedFrame != null)
                {
                    showStack(selectedFrame);
                } else {
                    //TODO: thjis should be prevented...
                }
                mStackText.append("</HTML>");
                taStack.setText(mStackText.toString());
            }
        }

            private void showStack(final Frame selectedFrame)
            {
                taStack.setText(null);
                
                for (Stack curStack : selectedFrame.getLeafStack())
                {
                    writeSingleStack (curStack);
                }
                
                for (Stack CurStack : selectedFrame.getParticipantStack())
                {
                    writeSingleStack (CurStack);
                }
            }

        private void writeSingleStack(Stack curStack)
        {    
            mStackText.append("Stack Hits: ");
            mStackText.append(curStack.getHits());
            mStackText.append("<BR>");
          
            int numOfSpaces = 0;
            for (Frame curFrame : curStack.getStackFrames())
            {
                for (int i = 0; i <= numOfSpaces; ++i)
                {
                    mStackText.append("&NBSP;");
                }   
                numOfSpaces++;
                if (curFrame.getAddress().compareTo(mSelectedAddress) == 0)
                {
                    mStackText.append("<B>");
                     mStackText.append(curFrame.getName());
                     mStackText.append("</B>");
                }  else {
                     mStackText.append(curFrame.getName());
                }
               
                mStackText.append("<BR>");
            }  
            mStackText.append("<HR>");
        }
        
        private final StringBuilder mStackText = new StringBuilder();
        private String mSelectedAddress = null;
    }
}