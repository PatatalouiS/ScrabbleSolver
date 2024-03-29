<h1> Introduction </h1>

<h4>⚠️ Please read entirely the Instructions for build the project and be aware of all the available options ️⚠️ </h4>

<h1> Documentation </h1>

All the full documentation of the code is available with the <code><b>doxygen</b></code> tool. If you don't have this tool, installing it is very simple, just do : 
<pre>$ sudo apt install doxygen       <i>//On Debian/Ubuntu Unix-like system</i></pre>
<pre>$ brew install doxygen           <i>//On OSX system</i></pre>

After making sure you have this tool, just do :
<pre>$ make doc</pre>

With this, all the documentation will be generated in the <code><b>doc/</b></code> folder. Now, you can easily access it by openning the file <code><b>doc/html/index.html</b></code>

<h1> Build </h1>

This project can be build on <b>Unix</b> and <b>OSX</b> Systems.

This project use a lot of C++17 features and must be build with a recent compiler. So please ensure you have :
<ul>
    <li> <b>GCC</b> v8.3.0 or latest </li>
    <li> <b>Clang</b> v11.0.0 or latest </li>
</ul>

After cloning or downloading the project, in the root of projet just do :
<pre>$ make</pre>

If all is OK, you should see an executable file named <code><b>ScrabbleSolver</b></code> in the <code><b>bin/</b></code> directory.

You can remove all the obj files created with :
<pre>$ make clean</pre>

Or remove all executables and obj files ( to retry a fail build for example ) with :
<pre>$ make superclean </pre>

<h1>Available options </h1>

Three options are available with command line args : 
<ul>
    <li><b><code>--loop</code></b>: Run a loop wich will successively resolve an empty scrabble board until no letters are available or that no move is playable. Just press ENTER Key to see the next best move.</li> <br>
    <li><b><code>--jokers</code></b> : Enable the use of jokers. Jokers are represented by '?' symbol in the file <code><b>config_board.txt</b></code> </li> <br>
    <li><b><code>--suzette-check</code></b> : Emit a query to the Suzette Bot of Mr Nivoliers for each move played and display to console. It's a good way for you to check the correctness of our software. You must execute a little bash script named <code><b>suzetteconnect.sh</b></code> to pass the University Firewall and allow the proper functioning of this option.</li>
</ul>

<h4>⚠️ Activation of the <code><b>--suzette-check</b></code> option </h4>
As explained, you must pass the Firewall of UCBL1 to keep the Suzette asserts working. If you are already physically on the University campus, you don't need to run the script. Else, just run the
script <code><b>suzetteconnect.sh</b></code>, <b>( and don't forget to pass tour UCLB1 login at first arg )</b> on another console window. After, you can use <code><b>--suzette-check</b></code> as you want. run this script from the root of the project with the command:<br>
<pre>$ sh suzetteconnect.sh [p1234567]  <i>//Your UCBL1 login</i></pre> 

This script is going to ask you a password for authenticate you at UCBL. The password to enter is your UCBL1 password. <b>The script will not return</b>, it's normal ! kill the process once you no longer need the option <code><b>--suzette-check</b></code>

<h4>⚠️ Warning</h4> The <code><b>--jokers</b></code> option and the <code><b>--suzette-check</b></code> option are not compatible together. Indeed, The Suzette Server can't play with jokers actually. You will be prompt by the software if you put these two options together. 

<h2>Run</h2>

<h4>⚠ All the executions must be run from the root of the project ⚠️</h4>
To run the Solver without any options, just ensure you are at the root of project and do :
<pre>$ ./bin/ScrabbleSolver</pre>

Without any options, the program load prints the enabled options, wait for ENTER Key, then loads the Configuration written in the file <code><b>data/config_board.txt</b></code>, shows the basic configuration and prints the best move for this configuration.

Options can be combinated together, for example if you run :
<pre>$ ./bin/ScrabbleSolver --loop --suzette-check</pre>
So, the loop will be executed with a Suzette check for all moves will be computed

The config file <code><b>data/config_board.txt</b></code> must respect the following conventions, you can edit it for solve all the configurations you want to test. if the <code><b>--jokers</b></code> option is enabled, you can add a joker to the 7 letters availables of the player by putting the '?' symbol (Maximum 2 jokers are allowed):
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
AEBDECZ          <i>// letters of the player, can contains 1 or 2 '?' symbols of joker</i>
</code></pre>

<h1>Third Party Libraries</h1>

Especially for the <code><b>--suzette-check</b></code> option, we used <a href="https://github.com/yhirose/cpp-httplib">httplib</a>
to send Queries to the Suzette server with an hhtp client and <a href="https://github.com/nlohmann/json">nlohmannjson</a> to parse the JSON query result easily. These to libraries are HTTP-header only, you don't need to install or download it.

<h1>Contact</h1>

If you have any problems for building or using certain options, please contact us : <a href="mailto:maxime.olivie@etu.univ-lyon1.fr">SUPPORT</a>

<h1>Authors</h1>

<ul>
    <li> CERBULEAN Ana-Maria P1710179 </li>
    <li> OLIVIE Maxime P1712754 </li>
</ul>
<br>
