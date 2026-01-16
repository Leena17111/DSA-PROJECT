#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// =====================
// Tree Node Definition
// =====================
struct TreeNode {
    string studname;

    double quiz;
    double assignment;
    double test1;
    double lab;
    double project;
    double exercise;
    double finalExam;
    double totalMark;
    char grade;

    TreeNode* left;
    TreeNode* right;
};

// =====================
// Statistic Struct
// =====================
struct Statistic {
    int count, passCount;
    double sum;

    TreeNode* highest;
    TreeNode* lowest;

    int gradeCount[5]; // A B C D F

    Statistic() {
        count = passCount = 0;
        sum = 0;
        highest = lowest = nullptr;
        for (int i = 0; i < 5; i++)
            gradeCount[i] = 0;
    }

    void Traverse(TreeNode* tree) {
        if (!tree) return;

        Traverse(tree->left);

        count++;
        sum += tree->totalMark;

        if (tree->totalMark >= 50)
            passCount++;

        if (!highest || tree->totalMark > highest->totalMark)
            highest = tree;

        if (!lowest || tree->totalMark < lowest->totalMark)
            lowest = tree;

        switch (tree->grade) {
        case 'A': gradeCount[0]++; break;
        case 'B': gradeCount[1]++; break;
        case 'C': gradeCount[2]++; break;
        case 'D': gradeCount[3]++; break;
        case 'F': gradeCount[4]++; break;
        }

        Traverse(tree->right);
    }

    void Display() const {
    if (count == 0) {
        cout << "No records available.\n";
        return;
    }

    cout << "\n===== Class Statistics =====\n\n";

    // Summary table
    cout << left
         << setw(20) << "Criteria"
         << setw(20) << "Value"
         << endl;

    cout << string(40, '-') << endl;

    cout << setw(20) << "Class Average"
         << setw(20) << fixed << setprecision(2)
         << sum / count << endl;

    cout << setw(20) << "Highest"
     << setw(10) << highest->studname
     << fixed << setprecision(2)
     << "(" << highest->totalMark << ")" << endl;


   cout << setw(20) << "Lowest"
     << setw(10) << lowest->studname
     << fixed << setprecision(2)
     << "(" << lowest->totalMark << ")" << endl;


   cout << setw(20) << "Pass Rate"
     << fixed << setprecision(2)
     << (double(passCount) / count) * 100 << "%" << endl;


    // Grade distribution table
    cout << "\nGrade Distribution\n";
    cout << string(40, '-') << endl;

    cout << left
         << setw(10) << "Grade"
         << setw(10) << "Count"
         << endl;

    cout << string(20, '-') << endl;

    cout << setw(10) << "A" << setw(10) << gradeCount[0] << endl;
    cout << setw(10) << "B" << setw(10) << gradeCount[1] << endl;
    cout << setw(10) << "C" << setw(10) << gradeCount[2] << endl;
    cout << setw(10) << "D" << setw(10) << gradeCount[3] << endl;
    cout << setw(10) << "F" << setw(10) << gradeCount[4] << endl;
}

};

// =====================
// Input validation helper
// =====================
double getMark(const string& label, double max)
{
    double value;
    do {
        cout << label;
        cin >> value;

        if (value < 0 || value > max)
            cout << "Invalid input. Enter value between 0 and "
                 << max << ".\n";
    } while (value < 0 || value > max);

    return value;
}

// =====================
// TreeType Class
// =====================
class TreeType {
public:
    TreeType();
    ~TreeType();

    bool IsEmpty() const;
    int NumberOfNodes() const;

    void InsertItem(string name,
        double quiz, double assignment, double test1,
        double lab, double project, double exercise,
        double finalExam);

    void RetrieveItem(string item, bool& found);
    void DeleteItem(string item);

    void PrintTree() const;
    void PrintInOrder() const;
    void PrintPreOrder() const;
    void PrintPostOrder() const;

    void ComputeStatistics() const;

private:
    TreeNode* root;

    void Destroy(TreeNode*& tree);
    void Insert(TreeNode*& tree, TreeNode* newNode);
    void Retrieve(TreeNode* tree, string item, bool& found) const;
    void Delete(TreeNode*& tree, string item);
    void DeleteNode(TreeNode*& tree);
    void GetPredecessor(TreeNode* tree, TreeNode*& pred);

    int CountNodes(TreeNode* tree) const;

    //Helper for displaying
    void PrintFullRecord(TreeNode* node) const;

    void PrintInOrder(TreeNode* tree) const;
    void PrintPreOrder(TreeNode* tree) const;
    void PrintPostOrder(TreeNode* tree) const;
};

// =====================
// Constructor / Destructor
// =====================
TreeType::TreeType() { root = nullptr; }
TreeType::~TreeType() { Destroy(root); }

// =====================
// IsEmpty & NumberOfNodes
// =====================
bool TreeType::IsEmpty() const {
    return root == nullptr;
}

int TreeType::NumberOfNodes() const {
    return CountNodes(root);
}

int TreeType::CountNodes(TreeNode* tree) const {
    if (!tree) return 0;
    return 1 + CountNodes(tree->left) + CountNodes(tree->right);
}

// Helper for displaying (TABLE FORMAT)
void TreeType::PrintFullRecord(TreeNode* node) const
{

    // Print one row
    cout << left
         << setw(15) << node->studname
         << setw(8)  << node->quiz
         << setw(12) << node->assignment
         << setw(8)  << node->test1
         << setw(6)  << node->lab
         << setw(9)  << node->project
         << setw(10) << node->exercise
         << setw(8)  << node->finalExam
         << setw(8)  << node->totalMark
         << setw(6)  << node->grade
         << endl;
}


// =====================
// Insert / Retrieve / Delete
// =====================
void TreeType::InsertItem(string name,
    double quiz, double assignment, double test1,
    double lab, double project, double exercise,
    double finalExam)
{
    TreeNode* n = new TreeNode;

    n->studname = name;
    n->quiz = quiz;
    n->assignment = assignment;
    n->test1 = test1;
    n->lab = lab;
    n->project = project;
    n->exercise = exercise;
    n->finalExam = finalExam;

    n->totalMark = quiz + assignment + test1 + lab +
                   project + exercise + finalExam;

    if (n->totalMark >= 80) n->grade = 'A';
    else if (n->totalMark >= 65) n->grade = 'B';
    else if (n->totalMark >= 50) n->grade = 'C';
    else if (n->totalMark >= 40) n->grade = 'D';
    else n->grade = 'F';

    n->left = n->right = nullptr;

    Insert(root, n);

    cout << "\nCurrent Tree (" << NumberOfNodes() << " students)\n";
}

void TreeType::RetrieveItem(string item, bool& found) {
    Retrieve(root, item, found);
}

void TreeType::DeleteItem(string item) {
    Delete(root, item);
    cout << "\nTree After Deletion (" << NumberOfNodes() << " students)\n";
}

void TreeType::Insert(TreeNode*& tree, TreeNode* newNode)
{
    if (!tree) {
        tree = newNode;
        return;
    }

    if (newNode->studname < tree->studname)
        Insert(tree->left, newNode);
    else if (newNode->studname > tree->studname)
        Insert(tree->right, newNode);
    else {
        cout << "Duplicate name not allowed: "
             << newNode->studname << endl;
        delete newNode;
    }
}

void TreeType::Retrieve(TreeNode* tree, string item, bool& found) const
{
    if (!tree) {
        found = false;
        return;
    }

    if (item == tree->studname) {
        found = true;

         cout << left
         << setw(15) << "Name"
         << setw(8)  << "Quiz"
         << setw(12) << "Assignment"
         << setw(8)  << "Test1"
         << setw(6)  << "Lab"
         << setw(9)  << "Project"
         << setw(10) << "Exercise"
         << setw(8)  << "Final"
         << setw(8)  << "Total"
         << setw(6)  << "Grade"
         << endl;

        cout << string(90, '-') << endl;

        PrintFullRecord(tree);
        return;
    }

    if (item < tree->studname)
        Retrieve(tree->left, item, found);
    else
        Retrieve(tree->right, item, found);
}

void TreeType::Delete(TreeNode*& tree, string item)
{
    if (!tree) return;

    if (item < tree->studname)
        Delete(tree->left, item);
    else if (item > tree->studname)
        Delete(tree->right, item);
    else
        DeleteNode(tree);
}

void TreeType::DeleteNode(TreeNode*& tree)
{
    TreeNode* temp = tree;

    if (!tree->left && !tree->right) {
        delete tree;
        tree = nullptr;
    }
    else if (!tree->left) {
        tree = tree->right;
        delete temp;
    }
    else if (!tree->right) {
        tree = tree->left;
        delete temp;
    }
    else {
        TreeNode* pred;
        GetPredecessor(tree->left, pred);
        *tree = *pred;
        Delete(tree->left, pred->studname);
    }
}

void TreeType::GetPredecessor(TreeNode* tree, TreeNode*& pred)
{
    while (tree->right)
        tree = tree->right;
    pred = tree;
}

// =====================
// Traversals
// =====================
void TreeType::PrintTree() const {
    if (IsEmpty()) {
        cout << "Tree is empty.\n";
        return;
    }
    PrintInOrder(root);
}

void TreeType::PrintInOrder() const { 
    cout << left
         << setw(15) << "Name"
         << setw(8)  << "Quiz"
         << setw(12) << "Assignment"
         << setw(8)  << "Test1"
         << setw(6)  << "Lab"
         << setw(9)  << "Project"
         << setw(10) << "Exercise"
         << setw(8)  << "Final"
         << setw(8)  << "Total"
         << setw(6)  << "Grade"
         << endl;

    cout << string(90, '-') << endl;
    
    PrintInOrder(root); }

void TreeType::PrintInOrder(TreeNode* tree) const {
    if (!tree) return;
    PrintInOrder(tree->left);
    PrintFullRecord(tree);
    PrintInOrder(tree->right);
}

void TreeType::PrintPreOrder() const { 
    cout << left
         << setw(15) << "Name"
         << setw(8)  << "Quiz"
         << setw(12) << "Assignment"
         << setw(8)  << "Test1"
         << setw(6)  << "Lab"
         << setw(9)  << "Project"
         << setw(10) << "Exercise"
         << setw(8)  << "Final"
         << setw(8)  << "Total"
         << setw(6)  << "Grade"
         << endl;

    cout << string(90, '-') << endl;
    
    PrintPreOrder(root); }

void TreeType::PrintPreOrder(TreeNode* tree) const {
    if (!tree) return;
    PrintFullRecord(tree);
    PrintPreOrder(tree->left);
    PrintPreOrder(tree->right);
}

void TreeType::PrintPostOrder() const { 
    cout << left
         << setw(15) << "Name"
         << setw(8)  << "Quiz"
         << setw(12) << "Assignment"
         << setw(8)  << "Test1"
         << setw(6)  << "Lab"
         << setw(9)  << "Project"
         << setw(10) << "Exercise"
         << setw(8)  << "Final"
         << setw(8)  << "Total"
         << setw(6)  << "Grade"
         << endl;

    cout << string(90, '-') << endl;
    
    PrintPostOrder(root); }
void TreeType::PrintPostOrder(TreeNode* tree) const {
    if (!tree) return;
    PrintPostOrder(tree->left);
    PrintPostOrder(tree->right);
    PrintFullRecord(tree);
}

// =====================
// Statistics
// =====================
void TreeType::ComputeStatistics() const {
    Statistic s;
    s.Traverse(root);
    s.Display();
}

// =====================
// Destroy
// =====================
void TreeType::Destroy(TreeNode*& tree)
{
    if (!tree) return;
    Destroy(tree->left);
    Destroy(tree->right);
    delete tree;
    tree = nullptr;
}

// =====================
// Main (Menu)
// =====================
int main()
{
    TreeType bst;
    int choice;
    do {
        cout << "\n1. Insert student record"
             << "\n2. Display students (Inorder)"
             << "\n3. Display students (Preorder)"
             << "\n4. Display students (Postorder)"
             << "\n5. Search student by name"
             << "\n6. Delete student by name"
             << "\n7. Class statistics"
             << "\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "Name: ";
            cin.ignore();
            getline(cin, name);

            double q = getMark("Quiz (10%): ", 10);
            double a = getMark("Assignment (10%): ", 10);
            double t1 = getMark("Test1 (20%): ", 20);
            double l = getMark("Lab (15%): ", 15);
            double p = getMark("Project (10%): ", 10);
            double e = getMark("Exercise (5%): ", 5);
            double f = getMark("Final (30%): ", 30);

            bst.InsertItem(name, q, a, t1, l, p, e, f);
        }
        
        else if (choice == 2) bst.PrintInOrder();
        else if (choice == 3) bst.PrintPreOrder();
        else if (choice == 4) bst.PrintPostOrder();
        else if (choice == 5) {
            string name;
            bool found;
            cout << "Search name: ";
            cin.ignore();
            getline(cin, name);
            bst.RetrieveItem(name, found);
            if (!found) cout << "Student record with the name "<<'"'<<name<<'"'<<" is not found.\n";
        }

        else if (choice == 6) {
        string name;
        bool found = false;

        cout << "Delete name: ";
        cin.ignore();
        getline(cin, name);

        bst.RetrieveItem(name, found);

        if (!found) {
            cout << "Student record with the name "<<'"'<<name<<'"'<<" is not found. Deletion aborted.\n";
        } else {
            bst.DeleteItem(name);
        }
    }

        else if (choice == 7)
            bst.ComputeStatistics();
        else {
            if (choice != 0)
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
