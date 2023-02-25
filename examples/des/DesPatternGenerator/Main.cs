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
			Int32 numOfInputs = 200000;
			byte[] bytes = new byte[16];
			UInt64 pattern;
			Random rnd = new Random();
			for(Int32 i = 0 ; i < numOfInputs ; i++)
			{
				rnd.NextBytes(bytes);
				pattern = BitConverter.ToUInt64(bytes, 0);
				patterns.AppendFormat("{0}", pattern);

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
