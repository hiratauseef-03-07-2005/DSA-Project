Hangman Game (DSA Project in C)

This is a console-based Hangman game developed in C language as a Data Structures project. The game selects a random word and the player has limited attempts to guess the correct word letter by letter.

The project uses multiple data structures such as Binary Search Tree (BST), Stack, Linked List, and Graph.

Main Features:

->Random word selection from a Binary Search Tree.

->Hangman drawing display based on wrong guesses.

->Guess tracking using Stack.

->Score system (+2 for correct guess, -1 for wrong guess).

->Clue system (player can enter '?' to view clue).

->Related word system using Word Relationship Graph.

->Leaderboard system with file handling.

Data Structures Used:

->Binary Search Tree (BST):

Stores words and their clues. Used to insert and search words. Random word is selected from BST.

->Stack:

Stores guessed letters. Prevents guessing the same letter again.

->Graph (Adjacency List):

Stores relationship between words. After guessing a word, player can choose a related word.

->Linked List:

Used to store leaderboard details in sorted order.

Files Used:

->leaderboard.txt

Stores player names and scores permanently.

Game Menu Options:

1. Start Game
   
2. Show Leaderboard
   
3. Exit Game

Author

Hira Tauseef
