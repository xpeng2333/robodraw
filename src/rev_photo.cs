﻿using System;

namespace TCPServer
{
    class Program
    {
        static void Main(string[] args)
        {
            /* File name for received picture */
            const String FILE_NAME = "../data/img/picture.jpg";

            /* Port for incoming connections */
            const int PORT = 3800;

            /* The IPEndPoint for the server. IP cannot be localhost */
            System.Net.IPEndPoint remoteIpEndPoint = new System.Net.IPEndPoint(System.Net.IPAddress.Any, PORT);
            Console.WriteLine("Listening for connections on port " + PORT.ToString());

            /* After this amount of time has passed, any connection will be terminated
             * Keep high for high latency networks and vice versa */
            const int TIMEOUT = 1000;

            /* Start listening for connections */
            System.Net.Sockets.TcpListener tcpListener = new System.Net.Sockets.TcpListener(remoteIpEndPoint);
            tcpListener.Start();

            /* Create the listening thread */
            Console.WriteLine("Creating the Child Thread");
            System.Threading.Thread connectThread = new System.Threading.Thread(() => StartConnect(tcpListener, TIMEOUT, FILE_NAME));
            connectThread.Start();

            /* Terminate on keypress */
            Console.ReadKey();
            connectThread.Abort();

            /* Clean up and open the received file */
            tcpListener.Stop();
//            System.Diagnostics.Process.Start(FILE_NAME);
        }

        public static void StartConnect(System.Net.Sockets.TcpListener tcpListener, int TIMEOUT, String FILE_NAME)
        {
            try
            {
                while (true)
                {
                    /* Create a buffer for receiving */
                    byte[] receiveBytes = new byte[1024];

                    /* The socket that will be used for listening */
                    System.Net.Sockets.Socket sock = null;

                    /* FileStream for writing */
                    System.IO.FileStream objWriter = null;

                    /* Number and total number of bytes read till the end of loop */
                    int bytesRead = 0;
                    int totalBytesRead = 0;

                    /* Loop till something is read */
                    while (totalBytesRead == 0)
                    {
                        /* Sleep for 100ms if no connection is being made */
                        while (!tcpListener.Pending()) System.Threading.Thread.Sleep(100);

                        sock = tcpListener.AcceptSocket();
                        Console.WriteLine("Accepted Connection");
                        sock.ReceiveTimeout = TIMEOUT;

                        /* Sleep for another 100ms to give the client time to respond */
                        System.Threading.Thread.Sleep(100);
                        int filesize = 0;
                        try
                        {
                            /* Receive the header, terminate if not found */
                            if ((bytesRead = sock.Receive(receiveBytes)) > 0)
                            {
                                string[] headers = System.Text.Encoding.ASCII.GetString(receiveBytes).Split('\n');
                                if (headers[0] == "HEADER")
                                {
                                    Console.WriteLine("Receiving file of size " + headers[1] + " bytes");
                                    Int32.TryParse(headers[1], out filesize);
                                }
                                else throw new Exception("No header received");
                            }
                            else throw new Exception("No header received");

                            while ((totalBytesRead != filesize) && (bytesRead = sock.Receive(receiveBytes, receiveBytes.Length, System.Net.Sockets.SocketFlags.None)) > 0)
                            {
                                /* Delete existing file to be safe */
                                if (objWriter == null)
                                {
                                    if (System.IO.File.Exists(FILE_NAME)) System.IO.File.Delete(FILE_NAME);
                                    objWriter = System.IO.File.OpenWrite(FILE_NAME);
                                }

                                objWriter.Write(receiveBytes, 0, bytesRead);

                                totalBytesRead += bytesRead;

                                /* Reduce buffer size if bytes left are less than it */
                                if (filesize - totalBytesRead < receiveBytes.Length)
                                {
                                    receiveBytes = new byte[filesize - totalBytesRead];
                                }
                            }
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e.Message);
                        }

                        /* Close everything */
                        sock.Close();
                        if (!(objWriter == null))
                        {
                            objWriter.Close();
                            objWriter = null;
                        }
                        Console.WriteLine("Closed Connection");
                    }
                }
            }
            catch (System.Threading.ThreadAbortException)
            {
                Console.WriteLine("Thread Abort");
            }
        }
    }
}
