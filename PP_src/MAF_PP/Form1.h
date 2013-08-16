
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <tchar.h>
#include <msclr\marshal_cppstd.h>
#include "..\PhotoPlayer\src\getimage.h"


#pragma once

#include "EditXML.h"

namespace MAF_PP {

	using namespace msclr::interop;
	using namespace System::Runtime::InteropServices;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	using namespace System::Xml;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	static int photoNum=1;

	public:
		
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			
			
		}

	


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	protected: 



	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripTextBox1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripTextBox2;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripTextBox3;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	public: System::Windows::Forms::TextBox^  textBox1;
	private: 

	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;

	protected: 





	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripTextBox1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripTextBox2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripTextBox3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->toolStripTextBox1, 
				this->toolStripMenuItem2, this->toolStripTextBox2, this->toolStripTextBox3});
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(50, 20);
			this->toolStripMenuItem1->Text = L"Menu";
			this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::toolStripMenuItem1_Click);
			// 
			// toolStripTextBox1
			// 
			this->toolStripTextBox1->Name = L"toolStripTextBox1";
			this->toolStripTextBox1->Size = System::Drawing::Size(144, 22);
			this->toolStripTextBox1->Text = L"Open";
			this->toolStripTextBox1->Click += gcnew System::EventHandler(this, &Form1::toolStripTextBox1_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(144, 22);
			this->toolStripMenuItem2->Text = L"New mp4 file";
			// 
			// toolStripTextBox2
			// 
			this->toolStripTextBox2->Name = L"toolStripTextBox2";
			this->toolStripTextBox2->Size = System::Drawing::Size(144, 22);
			this->toolStripTextBox2->Text = L"Save";
			// 
			// toolStripTextBox3
			// 
			this->toolStripTextBox3->Name = L"toolStripTextBox3";
			this->toolStripTextBox3->Size = System::Drawing::Size(144, 22);
			this->toolStripTextBox3->Text = L"Exit";
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripMenuItem1});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(995, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"Menu";
			this->menuStrip1->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Form1::menuStrip1_ItemClicked);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->Location = System::Drawing::Point(190, 40);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(478, 371);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button1->AutoSize = true;
			this->button1->Location = System::Drawing::Point(190, 430);
			this->button1->MaximumSize = System::Drawing::Size(184, 41);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(184, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"<<";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button2->Location = System::Drawing::Point(484, 430);
			this->button2->MaximumSize = System::Drawing::Size(184, 41);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(184, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L">>";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox1->BackColor = System::Drawing::Color::White;
			this->textBox1->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->textBox1->Location = System::Drawing::Point(683, 40);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(300, 371);
			this->textBox1->TabIndex = 4;
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(12, 59);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(159, 394);
			this->listBox1->TabIndex = 5;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::SystemColors::WindowText;
			this->label1->Location = System::Drawing::Point(52, 43);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"List of images";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(683, 430);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(286, 23);
			this->button3->TabIndex = 7;
			this->button3->Text = L"button3";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(995, 462);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Photo Player";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	public: System::Void toolStripTextBox1_Click(System::Object^  sender, System::EventArgs^  e) {
				 
				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
				 openFileDialog1->ShowDialog();
				 String^ file = (openFileDialog1->FileName);
				 char* inFile = (char*)(void*)Marshal::StringToHGlobalAnsi(file);	
				 int trc = getim(inFile);			
				 pictureBox1->Load("myphotos/photo1.jpg");
				 readXML(photoNum);

				 for(int i=1;i<=trc;i++)
					{
					listBox1->Items->Add("photo" + i);
					}
				
			 }

private: System::Void menuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
		 }
private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		 }

public: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		photoNum++; 
		ostringstream ss;
		ss << photoNum;
		string str = ss.str();
		string fileName= "myphotos/photo"+str+".jpg";
		ifstream jpgFile(fileName);

		if (jpgFile.good()) {
		
			pictureBox1->Load("myphotos/photo"+photoNum+".jpg");
			readXML(photoNum);	 
		}
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		
			if (photoNum != 1) {
			photoNum--; 
			pictureBox1->Load("myphotos/photo"+photoNum+".jpg");
			readXML(photoNum);
			}
				
		 }

public: void readXML(int num){

			XmlTextReader^ reader = gcnew XmlTextReader ("myphotos/"+num+".xml");
			textBox1->Text = "";

    while (reader->Read()) 
    {
        switch (reader->NodeType) 
        {
			case XmlNodeType::Element: // The node is an element.
				//Console::Write("<{0}", reader->Name);
				textBox1->AppendText(reader->Name);
				textBox1->AppendText(": ");
                while (reader->MoveToNextAttribute()) // Read the attributes.
					//Console::Write(" {0}='{1}'", reader->Name, reader->Value);
				//Console::WriteLine(">");
				textBox1->AppendText(reader->Name);
				textBox1->AppendText(": ");
				textBox1->AppendText(reader->Value);
				textBox1->AppendText("\r\n");
                break;
			case XmlNodeType::Text: //Display the text in each element.
				//Console::WriteLine (reader->Value);
				textBox1->AppendText(reader->Value);
				textBox1->AppendText("\r\n");
                break;
			//case XmlNodeType::EndElement: //Display the end of the element.
			//	//Console::Write("</{0}", reader->Name);
			//	//Console::WriteLine(">");
			//	textBox1->AppendText(reader->Name);
			//	textBox1->AppendText("\r\n");
   //             break;
				
        }
		textBox1->SelectionStart = 0;
		textBox1->ScrollToCaret();
		textBox1->Refresh();
    }
			
		}
private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			  int itemNum = listBox1->SelectedIndex;
			  photoNum = ++itemNum;
			  textBox1->Text = photoNum.ToString();
			  pictureBox1->Load("myphotos/photo"+photoNum+".jpg");
		      readXML(photoNum);
		 }

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			/* if (textBox1->Text!="") { 
				EditXML ^ edit = gcnew EditXML;
				edit->ShowDialog();
			 }
	*/
			OpenFileDialog^ openFileDialog2 = gcnew OpenFileDialog;
				 openFileDialog2->ShowDialog();
				 String^ xmlName = (openFileDialog2->FileName);
			System::Xml::XmlDocument xmlDoc;
			xmlDoc.Load(xmlName);

			// find desired node
			XmlNode ^node = xmlDoc.SelectSingleNode("//from");
			if (node != nullptr)
{
			node->InnerText="AGATA"; 
			}
			else
			{
				textBox1->Text="node not found";
			}
			xmlDoc.Save(xmlName);
			

		 }
};
}

