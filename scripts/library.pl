#! /usr/bin/perl -w

#
#   This will create functions of type:
#   int funcI(int arg1, int arg2)
#   {
#       arg1 += rand() % (30 + 3 * I);
#       arg2 += rand() % (100 + 2 * I);
#       arg1 += arg2 / arg1;
#       return arg1 + arg2;
#   }
#
#

if ($#ARGV != 0)
{
    print "Usage $0 num-functions\n";
    exit;
}
open (BIGLIB , ">bigLib.cc") or die "Couldn't open bigLib.cc for writing";

print BIGLIB "#include <cstdio>\n";
print BIGLIB "#include <cstdlib>\n";

for (my $i = 0; $i < $ARGV[0]; $i++)
{
    $num1 = 30 + $i * 3;
    $num2 = 100 + $i * 2;
    print BIGLIB "int func$i(int arg1, int arg2)\n";
    print BIGLIB "{\n";
    print BIGLIB "arg1 += rand() % $num1;\n";
    print BIGLIB "arg2 += rand() % $num2;\n";
    print BIGLIB "arg1 += arg2 / arg1;\n";
    print BIGLIB "return arg1 + arg2;\n";
    print BIGLIB "}\n\n";
    if ($i % 10000 == 0)
    {
       print "Processed chunk of 10000 functions\n"; 
    }
}

print "Compiling the generated library\n";

# Compile bigLib.cc
#`g++ -c bigLib.cc`;
# trying gcc to generate a shared library out of this
`gcc -fPIC -g -c bigLib.cc -o bigLib.o`;

# Print size for generated bigLib.o
my $report = `size bigLib.o`;
print "Generated bigLib.o with following size information:\n";
print "$report";
