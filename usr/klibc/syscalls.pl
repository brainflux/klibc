#!/usr/bin/perl
#
# Script to parse the SYSCALLS file and generate appropriate
# stubs.
#
# Pass 1: generate the C array of sizes
# Pass 2: generate the syscall stubs and other output
#

$v = $ENV{'KBUILD_VERBOSE'};
$quiet = defined($v) && ($v == 0) ? 1 : undef;

@args = ();
undef $pass;
for $arg ( @ARGV ) {
    if ( $arg =~ /^-/ ) {
	if ( $arg eq '-q' ) {
	    $quiet = 1;
	} elsif ( $arg eq '-v' ) {
	    $quiet = 0;
	} elsif ( $arg =~ /\-([0-9]+)$/ ) {
	    $pass = $1+0;
	} else {
	    die "$0: Unknown option: $arg\n";
	}
    } else {
	push(@args, $arg);
    }
}
($file, $sysstub, $arch, $bits, $unistd, $outputdir,
 $havesyscall, $typesize) = @args;

if (!$pass) {
    die "$0: Need to specify pass\n";
}

$quiet = ($pass != 2) unless defined($quiet);

require "$sysstub";

if (!open(UNISTD, '<', $unistd)) {
    die "$0: $unistd: $!\n";
}

while ( defined($line = <UNISTD>) ) {
    chomp $line;

    if ( $line =~ /^\#\s*define\s+__NR_([A-Za-z0-9_]+)\s+(.*\S)\s*$/ ) {
	$syscalls{$1} = $2;
	print STDERR "SYSCALL FOUND: $1\n" unless ( $quiet );
    }
}
close(UNISTD);

if ($pass == 2) {
    use bytes;

    if (!open(TYPESIZE, '<', $typesize)) {
	die "$0: $typesize: $!\n";
    }
    
    binmode TYPESIZE;

    $len = -s TYPESIZE;
    if (read(TYPESIZE, $typebin, $len) != $len) {
	die "$0: $typesize: short read: $!\n";
    }
    close(TYPESIZE);

    $ix = index($typebin, "\x7a\xc8\xdb\x4e\x97\xb4\xc9\x19");
    if ($ix < 0) {
	die "$0: $typesize: magic number not found\n";
    }

    # Remove magic number and bytes before it
    $typebin = substr($typebin, $ix+8);

    @size_table = unpack("C*", $typebin);
}

if ($pass == 2) {
    if (!open(HAVESYS, '>', $havesyscall)) {
	die "$0: $havesyscall: $!\n";
    }
    
    print HAVESYS "#ifndef _KLIBC_HAVESYSCALL_H\n";
    print HAVESYS "#define _KLIBC_HAVESYSCALL_H 1\n\n";
}

if (!open(FILE, '<', $file)) {
    die "$0: $file: $!\n";
}


if ($pass == 2) {
    print "syscall-objs := ";
}


# List here any types which should be sized even if they never occur
# in any system calls at all.
@type_list = ('int', 'long', 'void *', 'intptr_t', 'uintptr_t',
	      'intmax_t', 'uintmax_t');

%type_index = ();
%typesize = ();
$n = 0;
foreach $t (@type_list) {
    $type_index{$t} = $n;
    if ($pass == 2) {
	$typesize{$t} = $size_table[$n];
    }
    $n++;
}

while ( defined($line = <FILE>) ) {
    chomp $line;
    $line =~ s/\s*(|\#.*|\/\/.*)$//; # Strip comments and trailing blanks
    next unless $line;

    if ( $line =~ /^\s*(\<[^\>]+\>\s+|)([A-Za-z0-9_\*\s]+)\s+([A-Za-z0-9_,]+)(|\@[A-Za-z0-9_]+)(|\:\:[A-Za-z0-9_]+)\s*\(([^\:\)]*)\)\s*\;$/ ) {
	$archs  = $1;
	$type   = $2;
	$snames = $3;
	$stype  = $4;
	$fname  = $5;
	$argv   = $6;

	$doit  = 1;
	$maybe = 0;
	if ( $archs ne '' ) {
	    die "$file:$.: Invalid architecture spec: <$archs>\n"
		unless ( $archs =~ /^\<(|\?)(|\!)([^\>\!\?]*)\>/ );
	    $maybe = $1 ne '';
	    $not = $2 ne '';
	    $list = $3;

	    $doit = $not || ($list eq '');

	    @list = split(/,/, $list);
	    foreach  $a ( @list ) {
		if ( $a eq $arch || $a eq $bits ) {
		    $doit = !$not;
		    last;
		}
	    }
	}
	next if ( ! $doit );

	undef $sname;
	foreach $sn ( split(/,/, $snames) ) {
	    if ( defined $syscalls{$sn} ) {
		$sname = $sn;
		last;
	    }
	}
	if ( !defined($sname) ) {
	    next if ( $maybe );
	    die "$file:$.: Undefined system call: $snames\n";
	}

	$type  =~ s/\s*$//;
	$stype =~ s/^\@//;

	if ( $fname eq '' ) {
	    $fname = $sname;
	} else {
	    $fname =~ s/^\:\://;
	}

	@args = split(/\s*\,\s*/, $argv);

	# Assign types indexes in order of appearance, so that in pass 2
	# we know where to find it.
	foreach $a (@args) {
	    if (!defined($type_index{$a})) {
		$a_index = scalar(@type_list);
		push(@type_list, $a);
		$type_index{$a} = $a_index;
		if ($pass == 2) {
		    $typesize{$a} = $size_table[$a_index];
		}
	    }
	    if ($pass == 2) {
		print STDERR "sizeof($a) = ", $typesize{$a}, "\n";
	    }
	}

	if ($pass == 2) {
	    print HAVESYS "#define _KLIBC_HAVE_SYSCALL_${fname} ${sname}\n";
	    print " \\\n\t${fname}.o";
	    make_sysstub($outputdir, $fname, $type, $sname, $stype, @args);
	}
    } else {
	die "$file:$.: Could not parse input: \"$line\"\n";
    }
}

if ($pass == 1) {
    if (!open(TYPESIZE, '>', $typesize)) {
	die "$0: cannot create file: $typesize: $!\n";
    }

    print TYPESIZE "#include \"syscommon.h\"\n";
    print TYPESIZE "\n";
    print TYPESIZE "const unsigned char type_sizes[] = {\n";
    print TYPESIZE "\t0x7a,0xc8,0xdb,0x4e,0x97,0xb4,0xc9,0x19, /* magic */\n";
    foreach $t (@type_list) {
	print TYPESIZE "\tsizeof($t),\n";
    }
    print TYPESIZE "};\n";
    close(TYPESIZE);
} else {
    print "\n";

    print HAVESYS "\n#endif\n";
    close(HAVESYS);
}

