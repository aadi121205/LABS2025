#include <bits/stdc++.h>
using namespace std;

class NFA
{
public:
    int states, symbols;
    int startState;
    vector<int> finalStates;
    vector<vector<vector<int>>> transitions;

    NFA(int n, int s)
    {
        states = n;
        symbols = s;
        transitions.assign(states, vector<vector<int>>(symbols));
    }

    void addTransition(int from, int symbol, vector<int> toStates)
    {
        transitions[from][symbol] = toStates;
    }

    void setFinalStates(vector<int> finals)
    {
        finalStates = finals;
    }

    void setStartState(int start)
    {
        startState = start;
    }
};

class DFA
{
public:
    vector<set<int>> states;
    map<pair<int, int>, int> transitions;
    set<int> finalStates;
    int startState;

    void print()
    {
        cout << endl;
        cout << "DFA States:" << endl;
        for (int i = 0; i < states.size(); i++)
        {
            cout << "State " << i << " = { ";
            for (int s : states[i])
                cout << s << " ";
            cout << "}" << endl;
        }

        cout << "DFA Transitions:" << endl;
        for (auto &t : transitions)
        {
            cout << "T(" << t.first.first << ", " << char('a' + t.first.second) << ") = " << t.second << endl;
        }
        cout << endl;
        cout << "DFA Start State: " << startState << endl;

        cout << "DFA Final States: ";
        for (int f : finalStates)
            cout << f << " ";
        cout << endl;
    }
};

class NFAToDFAConverter
{
public:
    DFA convert(NFA &nfa)
    {
        DFA dfa;
        map<set<int>, int> stateId;
        queue<set<int>> q;

        set<int> start = {nfa.startState};
        dfa.startState = 0;
        dfa.states.push_back(start);
        stateId[start] = 0;
        q.push(start);

        while (!q.empty())
        {
            set<int> curr = q.front();
            q.pop();
            int currId = stateId[curr];

            for (int sym = 0; sym < nfa.symbols; sym++)
            {
                set<int> next;
                for (int st : curr)
                {
                    for (int nxt : nfa.transitions[st][sym])
                    {
                        next.insert(nxt);
                    }
                }
                if (!next.empty())
                {
                    if (stateId.find(next) == stateId.end())
                    {
                        int newId = dfa.states.size();
                        dfa.states.push_back(next);
                        stateId[next] = newId;
                        q.push(next);
                    }
                    dfa.transitions[{currId, sym}] = stateId[next];
                }
            }
        }

        for (auto &p : stateId)
        {
            for (int f : nfa.finalStates)
            {
                if (p.first.count(f))
                {
                    dfa.finalStates.insert(p.second);
                }
            }
        }

        return dfa;
    }
};

int main()
{
    int n, s;
    cout << "Enter number of states in NFA: ";
    cin >> n;
    cout << "Enter number of input symbols: ";
    cin >> s;

    NFA nfa(n, s);

    cout << "Enter transitions (format: from symbol to1 to2 ... -1):" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < s; j++)
        {
            cout << "T(" << i << ", " << char('a' + j) << "): ";
            vector<int> toStates;
            int x;
            while (cin >> x && x != -1)
            {
                toStates.push_back(x);
            }
            nfa.addTransition(i, j, toStates);
        }
    }

    int start;
    cout << "Enter start state: ";
    cin >> start;
    nfa.setStartState(start);

    int fCount;
    cout << "Enter number of final states: ";
    cin >> fCount;
    vector<int> finals(fCount);
    cout << "Enter final states: ";
    for (int i = 0; i < fCount; i++)
        cin >> finals[i];
    nfa.setFinalStates(finals);

    NFAToDFAConverter converter;
    DFA dfa = converter.convert(nfa);

    dfa.print();

    return 0;
}