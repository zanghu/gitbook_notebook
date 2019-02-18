#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) 
    {
        int delta = 0;
        int sum = 0;
        int val = 0;
        ListNode *p1 = l1;
        ListNode *p2 = l2;
        ListNode *p = NULL;
        ListNode *pre = NULL;
        ListNode *res = NULL;
        do {
            sum = p1->val + p2->val + delta;
            if (sum >= 10) {
                delta = 1;
                val = sum - 10;
            }
            else {
                delta = 0;
                val = sum;
            }
            p = new ListNode(val);
            if (pre) {
                pre->next = p;
            }
            else {
                res = p;
            }
            pre = p;
            p1 = p1->next;
            p2 = p2->next;
        } while (p1 && p2);

        ListNode * p3 = p1? p1: p2;
        while (p3) {
            sum = p3->val + delta;
            if (sum >= 10) {
                delta = 1;
                val = sum - 10;
            }
            else {
                delta = 0;
                val = sum;
            }
            p = new ListNode(val);
            if (pre) {
                pre->next = p;
            }
            pre = p;
            p3 = p3->next;
        }

        if (delta) {
            p = new ListNode(1);
            pre->next = p;
        }
        return res;
    }
};

int main()
{
    return 0;
}