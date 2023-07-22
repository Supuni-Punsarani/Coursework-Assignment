


# File Compresssion Machine


![th](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/9de31a73-4f72-411d-bbc4-6c53ec3ffe19)![th (2)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/c28e061f-f227-4f60-a840-076865341e55)


# Introduction


    The C program presented here is a simple file compression utility based on the Huffman coding algorithm. Huffman coding is a widely used lossless data compression technique that creates variable-length codes for characters based on their frequencies in the input file. The more frequent characters are assigned shorter codes, leading to efficient compression.

# purpose




    The purpose of this code is to implement a file compression utility using the Huffman coding algorithm.


# Objectives

- compression
- Huffman Tree Construction
- Symbol Encoding
- Error Handling
- User Interaction

# Code Structure

- Header Definitions: The required header files and constants are defined.
-  Data Structures: The program defines structures to represent the Huffman tree and bit buffer.
- Helper Functions: Utility functions are provided for error handling, creating and destroying Huffman nodes, and comparing nodes during tree construction.
- Huffman Tree Construction: The program calculates character frequencies, builds the Huffman tree, and constructs the Huffman table.
- Symbol Encoding: The program encodes each character of the input file using the Huffman table.
-  Compression: The encoded data is written to the output file in chunks.
- Main Function: The main function takes user input for input and output file names, compresses the file, and provides feedback on the compression success.

# Execution






- The user is prompted to enter the name of the input file to be compressed.
-  The user is prompted to enter the name of the output file to which the compressed data will be written.
-  The program reads the input file, calculates character frequencies, and constructs the Huffman tree.
- The input file is encoded using the Huffman table, and the compressed data is written to the output file.
- If the compression is successful, the program displays "File compressed successfully."
- If any error occurs during compression, the program displays "Error: Unable to compress the file. Please try again."



# Example

- Run the code


![Screenshot (43)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/43df7a9d-6b88-492a-8812-44f51b2742a3)

- Given the text file name in the location of source code.


![Screenshot (46)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/7e41799b-8c7e-4915-bc29-669748eb0793)

![Screenshot (45)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/dec6f66b-d420-4f2b-856b-9753a5ed023c)

- Given the name of output file.


![Screenshot (47)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/c954631b-1df8-4228-988b-d331e9af82a8)

- Finally, compressed the file successfully.


![Screenshot (49)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/dbf6bfdc-268d-4674-96f8-52be2c54a363)

- If the input or output file doesn’t exist in the location of source code,
  then gives the error message.


 ![Screenshot (50)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/5b2cdf03-a950-4700-a791-d2673cbd98bb)
 
![Screenshot (51)](https://github.com/Supuni-Punsarani/Coursework-Assignment/assets/114279717/9c6fb068-ee76-4426-9079-58a8feb55b4d)

# Conclusion

    The Huffman coding-based file compression utility presented in this program provides a basic demonstration of file compression principles. While it successfully compresses text files, there is room for improvement in terms of handling other file formats, optimizing performance, and providing more sophisticated error handling. Overall, the program showcases the power of Huffman coding in achieving lossless data compression, which has practical applications in various data storage and transmission scenarios.














                                       
