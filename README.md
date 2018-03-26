# sort-c

Boston area tech firms (and tech firms elsewhere) have found that the 
simplest
way to avoid hiring older employees is to have screeners, who are just
a few years out of college.

These interviewers ask quizzes that are easy for recent graduates, who 
are used to being given some pointless quizzes and who are up on the
latest terminology, which changed a lot as web application programming 
for web services became more common. 

The terminology has changed because
the IT coders that developed the current terminology knew very little Computer Science (CS) 
and thought they were newly developing concepts that have been common in CS for the
last 40 or 50 years.

At MIT we always said, "Whosoever doth not know [MULTICS](https://books.google.com/books?id=S8fHZzD5mbYC) is doomed to 
reinvent it." (We used more modern English.)

Because I learned to program on an [IBM 7090](https://en.wikipedia.org/wiki/IBM_7090), 
I have the background
to state computer science (CS) has not changed at all. (I am not so old. 
I learned to program in lower school. The main ideas of computer 
and software architechture go back to the 50s and predate me.) 

Nowadays
IT coders, who actually know very little CS, are judging those
that have pre-millennium degrees in CS, in Electrical Engineering (EE), or in Applied Mathematics.


Anyway, when one faces an interviewer that has barely more than a high
school understanding of CS and when the interviewer asks a question
about sorting or searching, probably one has to forget everything one
knows about [sorting and searching algorithms](https://brilliant.org/wiki/sorting-algorithms/) 
and must assume 
huge memory and compute power of a web server even if many systems
one might have to program are memory or CPU limited. 

With
assumptions of oodles of memory and compute power, one can sort by hashing 
(if needed) and by insertion
into (sparse) arrays.

This sorting program uses an indirected array to sort a [sparse
list of ordered pairs <x,y>](http://btechsmartclass.com/DS/U1_T14.html) of
integers by distance squared (the metric). I am using sparse list to
mean that the ordered pairs are not very close together.

I assumed 32 bit unsigned integers and thus the largest metric value is
4,294,967,295. It would take very little effort to make this program support a metric whose largest value is
1 less than 2 to the power of 64.

