#coding=utf-8
#!/bin/env python

import cscore

if __name__ == '__main__':

    StuScore = cscore.CScore("Kobe Bryant", 88, 100)

    StuScore.GetName()

    StuScore.GetMath()

    StuScore.GetEnglish()

    StuScore.CalTotalScore()
