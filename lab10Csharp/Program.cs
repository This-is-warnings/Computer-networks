using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace lab10Csharp
{
    class Program
    {
        static void Main(string[] args)
        {
            TcpClient tcp = new TcpClient("mail.ngs.ru", 110);
            NetworkStream ns = tcp.GetStream();
            StreamReader sr = new StreamReader(ns);
            string response = "GET /" + path + " HTTP/1.0\r\nHost: " + host + "\r\n\r\n";
            byte[] data = System.Text.Encoding.UTF8.GetBytes(response);
            ns.Write(data, 0, data.Length);
            string message = sr.ReadToEnd();     // считываем все сообщение

        }
    }
}
