#set -v
TERM=vt100
setenv() { export $1=$2 }  # csh compatibility

setenv LESS "-B -c -h4 -i -M -q -x4"

bindkey -em

path=($path d:/usr/local/wbin .)
PATH=path
cdpath=( C:/ D:/ )
fignore=(.obj .pdb .bsc .ilk .idb  .OBJ .PDB .BSC .ILK .IDB)

PROMPT='%3C>' #'%/>' #'%{f9%}%/%{gg%}(%{f2%}%?%{gg%})\>'
prompt3='%{fc%}Do you mean: %R ?(y|n|e)%{gg%} '

setopt autolist correctall automenu
setopt pushdtohome pushdsilent #autopushd
setopt nolistbeep

dirstacksize=100
histsize=1000
histfile=~/.zhistory


setopt notify monitor
setopt noclobber ignoreeof histignoredups autocd


alias	ls='ls -F'
alias 	h=history

#
alias 	rm='d:/usr/local/wbin/rm.exe -i'
alias 	rd='d:/usr/local/wbin/rmdir.exe'
alias 	md='d:/usr/local/wbin/mkdir.exe'
alias 	mv='d:/usr/local/wbin/mv.exe -i'
alias   cp='d:/usr/local/wbin/cp.exe -i'
alias	vi='nocorrect vi.exe'
alias	less='nocorrect less.exe'
alias   word='c:/apps/winword/winword/winword.exe'
alias   excel='e:/apps/excel/Excel/Excel.exe'
alias	emacs=c:/bin/emacs.csh
alias	gemacs=c:/bin/gemacs.csh #gui emacs
alias	helpcommand=winhlp32

bindkey ^W backward-delete-word

compctl -g '*(/)' cd pushd popd
compctl -g '*.zip' pkzip pkunzip
compctl -g '*.tar' tar
compctl -g '*.mak' nmake
compctl -g '*.hlp' winhlp32
compctl -l '' which where
compctl -g '*.tar' tar


#	complete winhlp32 'p/*/f:*.HLP/'
#	complete nmake 'p/*/f:*.MAK/'
#	complete bindkey 'p/*/b/'
#	complete set 'p/*/v/'
#	complete unset 'p/*/v/'

shares=(\\\\server1\\share1 \\\\server2\\share2  )

servers=(\\\\server1 \\\\server2 )

# complex completions
# some of these don't work too well

compctl -g '*.(tgz|gz)' -x \
	's[--]' -k '(stdout to-stdout decompress uncompress force help list license no-name quiet recurse suffix  test  verbose version fast best)' - \
	's[-S][--suffix]' -X '<file_name_suffix>' - \
	's[-]' -k "(c d f h l  L n q r S t v V 1 2 3 4 5 6 7 8 9 -)" -\
	's[-d][--(de|un)compress]' -g '*.{gz,Z,z,zip,taz,tgz}' - \
	-- gzip

compctl -g '*.(tgz|gz)' -x \
	's[-S][--suffix]' -X '<file_name_suffix>' - \
	's[-d][--(de|un)compress]' -g '*.{gz,Z,z,zip,taz,tgz}' - \
	's[-]' -k "(c d f h l  L n q r S t v V -)" -\
	's[--]' -k '(stdout to-stdout force help list license no-name quiet recurse suffix  test  verbose version )' - \
	-- gunzip

compctl  -k '(accounts computer config continue file group help helpmsg
	localgroup name pause print send session share start statistics stop time use user view)' -x \
	'c[-1,config]' -k '(server workstation)' - \
	'n[-1,/user:]' -k '(domain1\\amold domain2\\amold)' -\
	'c[-1,help]' -k '(accounts computer config continue file group help helpmsg localgroup name pause print send session share start statistics stop time use user view) ' -\
	 'c[-1,view]' -k servers -\
	 'c[-1,use]' -P "\* " -k shares -\
	 'c[-1,\\*]'  -k shares -\
	 -- net

compctl -g '*.(c|cpp)' -x \
	'n[1,-O]' -k '(1 2 a b d g i p s t w x y)' -\
	'n[1,-G]' -k '(3 4 5 B d r z e s f y h X)' - \
	'n[1,-F]' -k '(a A d e m o p r R)' - \
	'n[1,-Z]' -k '(i 7 d p a e g l z)' - \
	'n[1,-M]' -k '(T D L)' - \
'n[1,-]' -k (O G F C D E P  U u I X v Z \? c H J nologo T V w W Y M  L link) -\
	'c[-1,-link]' -s '`ls -1 d:/lang/msdev/lib`' -\
	-- cl

compctl -f -x\
	's[--]' -k '(backup force interactive update verbose suffix version-control help version)' -\
	's[-S][--suffix]' -X '<suffix>' -\
	's[-V][--version-control]' -k '(t numbered nil existing never simple)' -\
	's[-]'  -k '(b f i u v S V -)' -\
	 -- mv
#	n/-/f/ N/-/d/ p/1/f/ p/2/d/ n/*/f/
	
compctl -f -x\
	's[--]' -k '(archive backup no-dereference force interactive link preserve symbolic-link update verbose parents  one-file-system recursive suffix version-control help  version)' -\
	's[-]' -k '(a b d f i l p r s u v x P R S V -)' -\
	's[-*r]' -g '*(/)' -\
	's[-S][--suffix]' -X '<suffix>' -\
    's[-V][--version-control]' -k '(t numbered nil existing never simple)' -\
	-- cp

#n/-/f/ N/-/d/ p/1/f/ p/2/d/ n/*/f/
#end completions
