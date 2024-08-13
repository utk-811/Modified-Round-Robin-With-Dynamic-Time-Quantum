#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Process {
public:
    int pno;
    int at;
    int bt;
    int wt;
    int tat;
    int ct;
    int rt;

    Process(int pno, int at, int bt) {
        this->pno = pno;
        this->at = at;
        this->bt = bt;
        this->wt = 0;
        this->tat = 0;
        this->ct = at;
        this->rt = bt;
    }
};

vector<Process> input_processes() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    vector<Process> processes;
    for (int i = 0; i < n; i++) {
        int pno, at, bt;
        cout << "Enter process number, arrival time, and burst time: ";
        cin >> pno >> at >> bt;
        processes.emplace_back(pno, at, bt);
    }
    return processes;
}

void classical_rr(vector<Process> processes) {
    vector<Process> q = processes;
    int rbeg = 0, rend = 0;
    double l = 0, h = 0;
    int count = 0;
    int time = 0;
    int tq = 25;
    int time_exec = 0;
    double awt = 0, att = 0;

    while (count != processes.size()) {
        if (rend == 0) {
            for (int i = 0; i < processes.size(); i++) {
                if (q[i].at >= l && q[i].at <= h) {
                    rend++;
                }
            }
            if (rend == 0) {
                l = h;
                h += 1;
                continue;
            }
        }

        cout << "<" << time << ">---" << q[rbeg].pno << "---\n";

        time_exec = min(q[rbeg].rt, tq);
        q[rbeg].rt -= time_exec;
        q[rbeg].wt += time - q[rbeg].ct;
        time += time_exec;
        q[rbeg].ct = time;

        l = h + 0.001;
        h = h + time_exec;

        for (int i = 0; i < processes.size(); i++) {
            if (q[i].at >= l && q[i].at <= h) {
                rend++;
            }
        }

        if (q[rbeg].rt == 0) {
            count++;
            awt += q[rbeg].wt;
            q[rbeg].tat = q[rbeg].ct - q[rbeg].at;
            att += q[rbeg].tat;
            rbeg++;
        } else {
            q.push_back(q[rbeg]);
            rend++;
            rbeg++;
        }
    }

    cout << time << "\n\nAWT: " << awt / processes.size() << "  ATT: " << att / processes.size() << "\n";
}

void irr(vector<Process> processes) {
    vector<Process> q = processes;
    int rbeg = 0, rend = 0;
    double l = 0, h = 0;
    int count = 0;
    int time = 0;
    int tq = 25;
    int time_exec = 0;
    double awt = 0, att = 0;
    Process temp(0, 0, 0);
    double median = 0;
    int mid = 0;
    int n_temp = 0;
    int exec_temp = 0;

    while (count != processes.size()) {
        if (rend == 0) {
            for (int i = 0; i < processes.size(); i++) {
                if (q[i].at >= l && q[i].at <= h) {
                    rend++;
                    n_temp++;
                }
            }
            if (rend == 0) {
                l = h;
                h += 1;
                continue;
            }
        }

        for (int i = rbeg; i < rend; i++) {
            for (int j = rbeg; j < rend - 1; j++) {
                if (q[j].bt > q[j + 1].bt) {
                    temp = q[j];
                    q[j] = q[j + 1];
                    q[j + 1] = temp;
                }
            }
        }

        if (exec_temp == 0) {
            if ((rend - rbeg) % 2 == 0) {
                mid = (rend + rbeg) / 2;
                median = 0.5 * (q[mid].bt + q[mid - 1].bt);
            } else {
                mid = (rend + rbeg) / 2;
                median = q[mid].bt;
            }
            tq = (median + q[rend - 1].bt) / 2;
        }

        cout << "<" << time << ">---" << q[rbeg].pno << "---\n";
        exec_temp = (exec_temp + 1) % n_temp;

        time_exec = min(q[rbeg].rt, tq);
        q[rbeg].rt -= time_exec;
        q[rbeg].wt += time - q[rbeg].ct;
        time += time_exec;
        q[rbeg].ct = time;

        l = h + 0.001;
        h = h + time_exec;

        for (int i = 0; i < processes.size(); i++) {
            if (q[i].at >= l && q[i].at <= h) {
                rend++;
            }
        }

        if (q[rbeg].rt == 0) {
            count++;
            awt += q[rbeg].wt;
            q[rbeg].tat = q[rbeg].ct - q[rbeg].at;
            att += q[rbeg].tat;
            rbeg++;
        } else {
            q.push_back(q[rbeg]);
            rend++;
            rbeg++;
        }
    }

    cout << time << "\n\nAWT: " << awt / processes.size() << "  ATT: " << att / processes.size() << "\n";
}

int main() {
    vector<Process> processes = input_processes();
    cout << "\n\n\n\nClassical RR\n\n\n";
    classical_rr(processes);
    cout << "\n\n\n\nIRR\n\n\n";
    irr(processes);
    return 0;
}
