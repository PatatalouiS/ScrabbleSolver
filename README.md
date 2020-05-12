<img src='https://portraitsdefemmes.files.wordpress.com/2012/10/logo-ucbl.jpg?w=755' width='300'/> 
<h1> Introduction </h1>

<p>This project is an automatic Scrabble solver. It was realized for the LIFAP6 UE at UCBL1.<br>
To get the base project and all the statements follow <a href="https://forge.univ-lyon1.fr/lifap6/scrabble-etu">THIS LINK</a><p>

<h4>⚠️ Please read entirely the Instructions for build the project and be aware of all the available options ️⚠️ </h4>

<h1> Documentation </h1>

All the full documentation of the code is available with the <em> Doxygen </em> tool. yo can access to this documentation by the file <code><b> doc/html/index.html </b></code>

<h1> Build </h1>

This project can be build on Unix and OSX Systems.

This project use a lot of C++17 features and must be build with a recent compiler. So please ensure you have :
<ul>
    <li> <b>GCC</b> v8.3.0 or latest </li>
    <li> <b>Clang</b> v11.0.0 or latest </li>
</ul>

After cloning or downloading the project, in the root of projet just do :
<pre>$ make</pre>

If all is OK, you should see an executable file named <code>ScrabbleSolver</code> in the <code>bin/</code> directory.

You can remove all the obj files created with :
<pre>$ make clean</pre>

Or remove all executables and obj files ( to retry a fail build for example ) with :
<pre>$ make superclean </pre>

<h1>Available options </h1>

Three options are available with command line args : 
<ul>
    <li><b><code>--loop</code></b>: Run a loop wich will successively resolve an empty scrabble board until no letters are available or that no move is playable. Just press ENTER Key to see the next best move.</li> <br>
    <li><b><code>--jokers</code></b> : Enable the use of jokers. Jokers are represented by '?' symbol in the file <code>config_board.txt</code> </li> <br>
    <li><b><code>--suzette-check</code></b> : Emit a query to the Suzette Bot of Mr Nivoliers for each move played and display to console. It's a good way for you to check the correctness of our software. You must execute a little bash script named <code>suzetteconnect.sh</code> to pass the University Firewall and allow the proper functioning of this option.</li>
</ul>

<h4>⚠️ Activation of the <code>--suzette-check</code> option </h4>
As explained, you must pass the Firewall of UCBL1 to keep the Suzette asserts working. For this, just run the
script <code>suzetteconnect.sh</code> on another console window. After, you can use --suzette-check as you want. run this script from the root ofthe project with :
<pre>$ sh suzetteconnect.sh</pre>

This script is going to ask you a password for authenticate you at UCBL. This password will be send to you via @-Mail.

<h4>⚠️ Warning</h4> The <code>--jokers</code> option and the <code>--suzette-check</code> option are not compatible together. Indeed, The Suzette Server can't play with jokers actually. You will be prompt by the software if you put these two options together. 

<h2>Run</h2>

<h4>⚠ All the executions must be run from the root of the project ⚠️</h4>
To run the Solver without any options, just ensure you are at the root of project and do :
<pre>$ ./bin/ScrabbleSolver</pre>

Without any options, the program load prints the enabled options, wait for ENTER Key, then loads the Configuration written in the file <code>data/config_board.txt</code>, shows the basic configuration and prints the best move for this configuration.

Options can be combinated together, for example if you run :
<pre>$ ./bin/ScrabbleSolver --loop --suzette-check</pre>
So, the loop will be executed with a Suzette check for all moves will be computed

The config file <code>data/config_board.txt</code> must respect the following conventions, you can edit it for solve all the configurations you want to test :
<pre><code>...............
...............
...............
...............
...............
...............
...............
....PROJET.....
.......O.......
.......U.......
.......E.......
.......U.......
.....SCRABBLE..
...............
...............
AEBDECZ
</code></pre>

<h1>Third Party Libraries</h1>

Especially for the <code>--suzette-check</code> option, we used <a href="https://github.com/yhirose/cpp-httplib">httplib</a>
to send Queries to the Suzette server with an hhtp client and <a href="https://github.com/nlohmann/json">nlohmannjson</a> to parse the JSON query result easily. These to libraries are HTTP-header only, you don't need to install or download it.

<h1>Authors</h1>

<ul>
    <li> CERBULEAN Ana-Maria P1710179 </li>
    <li> OLIVIE Maxime P1712754 </li>
</ul>
<br>
