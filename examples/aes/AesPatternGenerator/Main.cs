using System;
using System.Text;
using System.Collections;
using System.Xml;
using System.Reflection;
using System.IO;
using System.Text.RegularExpressions;

namespace AesPatternGenerator
{
	//--------------------------------------------------------------
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1
	{
		//----------------------------------------------------
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			StringBuilder patterns = new StringBuilder("");
			Int32 numOfInputs = 100000;
			byte[] bytes = new byte[16];
			Random rnd = new Random();
			for(Int32 i = 0 ; i < numOfInputs ; i++)
			{
				rnd.NextBytes(bytes);
				for(Int32 j = 0 ; j < 16 ; j++)
				{
					if(j == 15)
						patterns.AppendFormat("{0}", (Int32)bytes[j]);
					else
						patterns.AppendFormat("{0} ", (Int32)bytes[j]);
				}
				if(i != numOfInputs - 1)
					patterns.Append("\r\n");
			}
			PutFileContents("patterns.txt", patterns.ToString());
			Console.WriteLine(numOfInputs);
		}
		//----------------------------------------------------
		private static void PutFileContents(String filename, String contents)
		{
			StreamWriter sw = new StreamWriter(filename);
			sw.Write(contents);
			sw.Flush();
			sw.Close();
		}
		//----------------------------------------------------
	}
	//--------------------------------------------------------------
}
