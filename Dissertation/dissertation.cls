\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{dissertation}[2012/05/04, Dan Page <page@cs.bris.ac.uk>]

\RequirePackage{pgfkeys}
\RequirePackage{pgfopts}

\pgfkeys{/dissertation/.cd,
  % options
      author/.store in=\dissertation@author,     % string
  supervisor/.store in=\dissertation@supervisor, % string
      degree/.store in=\dissertation@degree,     % [BSc|MEng|MSci|MSc|PhD]
       title/.store in=\dissertation@title,      % string
    subtitle/.store in=\dissertation@subtitle,   % string
        type/.store in=\dissertation@type,       % string
        year/.store in=\dissertation@year,       % string
  % defaults
      author = {},
  supervisor = {},
      degree = {MEng},
       title = {},
    subtitle = {},
        type = {},
        year = {\the\year}
}

\ProcessPgfOptions{/dissertation}
\LoadClass[10pt, a4paper, twoside]{report}
% \renewcommand{\baselinestretch}{1.4}

% see http://www.ctan.org/tex-archive/macros/latex/required/graphics
\RequirePackage{graphicx}
% see http://www.ctan.org/tex-archive/macros/latex/required/amslatex/
\RequirePackage{amsmath}
\RequirePackage{amssymb}

% see http://www.ctan.org/tex-archive/macros/latex/contrib/adjustbox
\RequirePackage{adjustbox}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/etoolbox
\RequirePackage{etoolbox}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/fancyhdr
\RequirePackage{fancyhdr}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/geometry
\RequirePackage{geometry}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/datetime/
\RequirePackage{datetime}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/algorithm2e
\RequirePackage[algochapter]{algorithm2e}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/listings
\RequirePackage{listings}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/url
\RequirePackage{url}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/hyperref
\RequirePackage{hyperref}
% see http://www.ctan.org/tex-archive/macros/latex/contrib/lipsum
\RequirePackage{lipsum}

% see http://www.ctan.org/tex-archive/usergrps/dante/dtk
\RequirePackage{dtklogos}

\makeatletter

% =============================================================================

% configure algorithms
\DontPrintSemicolon

% configure listings
\lstset{basicstyle=\ttfamily,captionpos=b}
\renewcommand{\lstlistlistingname}{List of Listings}

% configure hyperref
\hypersetup{colorlinks=false,urlcolor=red, linktocpage=true}

% configure style of table of contents
\setcounter{tocdepth}{2}

% configure style of bibliography
\bibliographystyle{plain}

% configure style of pages
\geometry{a4paper,inner=4cm,outer=2.5cm,top=1.5cm,bottom=1.5cm}

\fancypagestyle{plain}{
  \fancyhf{}

  \renewcommand{\headrulewidth}{0.5pt}
  \renewcommand{\footrulewidth}{0.5pt}

  \fancyfoot[C]{\thepage}
}

\fancypagestyle{marked}{
  \fancyhf{}

  \renewcommand{\headrulewidth}{0.5pt}
  \renewcommand{\footrulewidth}{0.5pt}

  \fancyhead[LO]{\slshape \rightmark}
  \fancyhead[RE]{\slshape  \leftmark}

  \fancyfoot[C]{\thepage}
}

\pagestyle{plain}


% -----------------------------------------------------------------------------


\newcommand{\thickhline}{%
    \noalign {\ifnum 0=`}\fi \hrule height 1.4pt
    \futurelet \reserved@a \@xhline
}


\newcommand{\commRightArrow}[1]
{
	$\begin{array}{c}
		#1\\[-5pt]
		\xrightarrow{\hspace{2.2cm}}
	\end{array}$
}

\newcommand{\commLeftArrow}[1]
{
	$\begin{array}{c}
		#1\\[-5pt]
		\xleftarrow{\hspace{2.2cm}}
	\end{array}$
}

\renewcommand{\maketitle}{
  \newpage
  \thispagestyle{empty}

  %\ifcsempty{dissertation@type}{}{
  %  \adjustbox{padding={5pt},frame={1pt},right}{Dissertation Type: \dissertation@type}
  %}

  \vspace*{\fill}
  \begin{center}
          \includegraphics[scale=0.28]{bristolunilogo}\\
			\vspace{1cm}
			{\Large UNIVERSITY OF BRISTOL}\\
			\vspace{0.5cm}
			DEPARTMENT OF COMPUTER SCIENCE\\

                       \vspace{3cm}
                \textbf{\LARGE \dissertation@title}                        \\
  \ifcsempty{dissertation@subtitle}{}{{\Large \dissertation@subtitle \\}}
                       \vspace{1cm}
                 {\Large Author : \dissertation@author}                    \\[0.25cm]
                 {\Large Supervisor : \dissertation@supervisor}            \\

                       \vspace{1cm}
                \rule{0.5\textwidth}{0.5pt}
                       \vspace{1cm}

        A dissertation submitted to the University of Bristol
        in accordance with the requirements of the degree of
  \mbox{\ifcsstring{dissertation@degree}{BSc} {Bachelor of Science}{}%
        \ifcsstring{dissertation@degree}{MEng}{Master   of Engineering}{}%
        \ifcsstring{dissertation@degree}{MSci}{Master   of Science}{}%
        \ifcsstring{dissertation@degree}{MSc} {Master   of Science}{}%
        \ifcsstring{dissertation@degree}{PhD} {Doctor   of Philosophy}{}%
  } in the Faculty of Engineering.                                

                       \vspace{1cm}
                \rule{0.5\textwidth}{0.5pt}
                       \vspace{1cm} 

                         \today

  \end{center}
  \vspace*{\fill}
}

% define macro to generate UoB declaration

\newcommand{\makedecl}{
  \newpage
  \thispagestyle{plain}

  \chapter*{Declaration}

  This thesis is submitted to the University of Bristol in accordance 
  with the requirements of the degree of {\dissertation@degree} in the Faculty 
  of Engineering.  It has not been submitted for any other degree or diploma 
  of any examining body.  Except where specifically acknowledged, it is all 
  the work of the Author. 

  \vspace{6cm}

  \noindent {\dissertation@author}, \today
}

% define macros to mark front, main and back matter
% 
% \renewcommand{\frontmatter}{
%   \cleardoublepage
%   \pagestyle{plain}
%   \pagenumbering{roman}
% }
% \renewcommand{\mainmatter}{
%   \cleardoublepage
%   \pagestyle{marked}
%   \pagenumbering{arabic}
% }
% \renewcommand{\backmatter}{
%   \cleardoublepage
%   \pagestyle{marked}
% }

% define macros to consistently generate dissertation type field

\newcommand{\typemeng}[1]{%
  \ifstrequal{#1}{E}{enterprise}{}%
  \ifstrequal{#1}{R}{research}{}%
}%
\newcommand{\typemsc}[3]{%
  $(#1,#2,#3)$%
}%

% =============================================================================

\makeatother
