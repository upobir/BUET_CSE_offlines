package PublicOrg;

import java.util.Scanner;

import PublicOrg.Organizations.*;

public class Main{
    public static void main(String[] args) {
        JCC jcc = new JCC();

        jcc.addColleague(new JPDC(jcc));
        jcc.addColleague(new JRTA(jcc));
        jcc.addColleague(new JTRC(jcc));
        jcc.addColleague(new JWSA(jcc));

        System.out.println("All four services initiated through mediator");

        Scanner scanner = new Scanner(System.in);

        while(true){

            String name = scanner.next();

            if(name.equals("Exit"))
                break;

            Colleague colleague = jcc.getByName(name);

            if(colleague == null){
                System.err.println("Invalid organization name");
                continue;
            }

            String command = scanner.next();

            if(command.equals("SERVE")){
                colleague.serve();
            }
            else{
                colleague.request(command);
            }

        }

        scanner.close();
    }
}