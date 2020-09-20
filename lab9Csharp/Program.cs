using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace lab9Csharp
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Введите адрес RSS ленты(без протокола)");
            string s = Console.ReadLine(); // news.tut.by/rss/all.rss    я использовал этот сайт
            string host = "", path="";
            int j = 0;
            bool b = true;
            while(j!=s.Length) // разбиваем адрес на путь и хост
            {
                if (b)
                {
                    if (s[j] != '/') host += s[j];
                    else b = false;
                }
                else path += s[j];
                j++;
            }
            Console.Write("host " + host + "\npath " + path + "\n");
            //  TcpClient tcp = new TcpClient("news.tut.by", 80);
            TcpClient tcp = new TcpClient(host, 80);
            NetworkStream ns = tcp.GetStream();
            StreamReader sr = new StreamReader(ns);
            string response = "GET /"+path+" HTTP/1.0\r\nHost: "+host+"\r\n\r\n";
            byte[] data = System.Text.Encoding.UTF8.GetBytes(response);
            ns.Write(data, 0, data.Length);
            string message = sr.ReadToEnd();     // считываем все сообщение
            //Console.WriteLine(message);
            tcp.Close();
            int i = 0;
            bool writeT = false;
            bool writeL = false;
            while(i!=message.Length) // цикл выводит на экран поочерёдно новости и ссылки к ней, ориентируясь по тегам
            {
                if(message[i] == '<' && message[i+1] == 't' && message[i+2] == 'i'
                    && message[i+3] == 't' && message[i+4] == 'l' && message[i+5] == 'e'
                    && message[i+6] == '>')
                {
                    Console.Write("Новость:  ");
                    writeT = true;
                    i = i + 7;
                }
                if(writeT)
                {
                    if (message[i] == '<' && message[i + 1] == '/' && message[i + 2] == 't' && message[i + 3] == 'i'
                    && message[i + 4] == 't' && message[i + 5] == 'l' && message[i + 6] == 'e'
                    && message[i + 7] == '>')
                    {
                        writeT = false;
                        Console.WriteLine();
                    }
                    else Console.Write(message[i]);
                }

                if (message[i] == '<' && message[i + 1] == 'l' && message[i + 2] == 'i'
                    && message[i + 3] == 'n' && message[i + 4] == 'k' 
                    && message[i + 5] == '>')
                {
                    Console.Write("Ссылка:  ");
                    writeL = true;
                    i = i + 6;
                    
                }
                if (writeL)
                {
                    if (message[i] == '<' && message[i + 1] == '/' && message[i + 2] == 'l' && message[i + 3] == 'i'
                    && message[i + 4] == 'n' && message[i + 5] == 'k'
                    && message[i + 6] == '>')
                    {
                        writeL = false;
                        Console.WriteLine("\n");
                    }
                    else Console.Write(message[i]);
                }


                i++;
            }
        }
    }
}
