package sample;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.FileChooser;
import javafx.event.ActionEvent;

import javax.print.DocFlavor;
import java.io.File;
import java.net.URL;
import java.util.Observable;
import java.util.ResourceBundle;
import java.util.Scanner;

public class Controller implements  Initializable{

    private File textfile;

    private Scanner filestream;

    @FXML
    private TableView<Word> table;

    @FXML
    private TableColumn<Word, String> wordcolumn;

    @FXML
    private TableColumn<Word, Integer> freqcolumn;

    @FXML
    private void ChooseFile(ActionEvent event){
        FileChooser filewindow = new FileChooser();

        /* CHANGE THIS */
        //filewindow.setInitialDirectory(new File("F:\\Documents\\Java programs\\CSE\\src\\sample"));

        filewindow.getExtensionFilters().addAll(new FileChooser.ExtensionFilter("Text Files", "*.txt"));
        textfile = filewindow.showOpenDialog(null);
        if(textfile != null){
            Button button = (Button) event.getSource();
            button.setText(textfile.getName());
        }
    }

    @FXML
    private void CountFreq(ActionEvent event){
        if(textfile == null) return;
        try {
            filestream = new Scanner(textfile);
        }
        catch (Exception e){
            System.out.println("Error occured while opening the file.");
            return;
        }

        filestream = filestream.useDelimiter("[^A-Za-z0-9]+");

        ObservableList<Word> wordlist = FXCollections.observableArrayList();

        while(filestream.hasNext()) {
            String w = filestream.next();
            w = w.toLowerCase();
            InsertWord(wordlist, w);
        }

        table.setItems(wordlist);
        filestream.close();
    }

    private void InsertWord(ObservableList<Word> wordlist, String w){
        for(Word item: wordlist){
            if(item.getWord().equals(w)){
                item.Increment();
                return;
            }
        }
        wordlist.add(new Word(w, 1));
    }

    public void initialize(URL url, ResourceBundle rb){
        wordcolumn.setCellValueFactory(new PropertyValueFactory<>("word"));
        freqcolumn.setCellValueFactory(new PropertyValueFactory<>("count"));
    }
}
