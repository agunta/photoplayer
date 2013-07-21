
#include <fstream>
#include <string> 
#include <sstream>
#include <windows.h>
#include <tchar.h>
#pragma once

namespace MAF_PP {

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
	static int trackCount=1;

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
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
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
			this->pictureBox1->ImageLocation = L"C:\\Users\\Agatka\\Desktop\\photoplayer\\PP_src\\bin\\1.jpg";
			this->pictureBox1->Location = System::Drawing::Point(12, 40);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(656, 554);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button1->AutoSize = true;
			this->button1->Location = System::Drawing::Point(12, 609);
			this->button1->MaximumSize = System::Drawing::Size(184, 41);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(184, 41);
			this->button1->TabIndex = 2;
			this->button1->Text = L"<<";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button2->Location = System::Drawing::Point(484, 609);
			this->button2->MaximumSize = System::Drawing::Size(184, 41);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(184, 41);
			this->button2->TabIndex = 3;
			this->button2->Text = L">>";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox1->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->textBox1->Location = System::Drawing::Point(683, 40);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox1->Size = System::Drawing::Size(300, 610);
			this->textBox1->TabIndex = 4;
			this->textBox1->Text = resources->GetString(L"textBox1.Text");
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(995, 662);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Photo Player";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void toolStripTextBox1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
				 openFileDialog1->ShowDialog();
				 pictureBox1->Load(openFileDialog1->FileName);
			 }

private: System::Void menuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
		 }
private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		trackCount++; 
		ostringstream ss;
		ss << trackCount;
		string str = ss.str();
		string fileName= "C:\\Users\\Agatka\\Desktop\\photoplayer\\PP_src\\bin\\"+str+".jpg";
		ifstream jpgFile(fileName);

		if (jpgFile.fail()) {
			trackCount = 1;
		} 
			pictureBox1->Load("C:\\Users\\Agatka\\Desktop\\photoplayer\\PP_src\\bin\\"+trackCount+".jpg");
			//textBox1->Text=System::IO::File::ReadAllText("C:\\Users\\Agatka\\Desktop\\photoplayer\\PP_src\\bin\\"+trackCount+".xml");
			//string xmlFile =  "C:\\Users\\Agatka\\Desktop\\photoplayer\\PP_src\\bin\\"+str+".xml";
			XmlTextReader^ reader = gcnew XmlTextReader ("C:\\Users\\Agatka\\Desktop\\photoplayer\\PP_src\\bin\\2.xml");
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
    }
		 
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		
			trackCount--; 
			pictureBox1->Load("C:\\Users\\Agatka\\Desktop\\photoplayer\\PP_src\\bin\\"+trackCount+".jpg");
		

		
		 }
};
}

