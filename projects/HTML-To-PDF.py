# Copyright Matthew Boiarski 2025 #

# Simple Resume Generator, at least to my standards #
# Email: 21@boiarskim@gmail.com #

# Core Imports #
import subprocess
import sys

### AUTO IMPORT SECTION ###
required_packages = {
    'os': None,
    'subprocess': None,
    'asyncio': None,
    'tkinter': None,
    'pyppeteer': 'pyppeteer',
    'PIL': 'Pillow',
    'fitz': 'PyMuPDF',
}

def install_package(package):
    subprocess.check_call([sys.executable, "-m", "pip", "install", package])

def install_required_packages():
    for module, package in required_packages.items():
        try:
            __import__(module)
        except ImportError:
            if package:
                print(f"Installing {package}...")
                install_package(package)
            else:
                print(f"Module {module} is part of the standard library, no installation needed.")
        else:
            print(f"{module} is already installed.")

# Run the installation process, and grab each package
install_required_packages()

# Core
import os
import asyncio

# GUI Imports #
import tkinter as tk
from tkinter import filedialog, messagebox, ttk

# HTML-to-PDF and PDF Viewer Imports #
from pyppeteer import launch
from PIL import Image, ImageTk
import fitz  # or just PyMuPDF, the egregious naming of python libs is a horror that mankind created for itself

class PDFGeneratorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("PDF Generator")
        self.root.rowconfigure(0, weight=1)  
        self.root.columnconfigure(1, weight=1)
        left_frame = tk.Frame(root)
        left_frame.grid(row=0, column=0, sticky="ns", padx=10, pady=10)
        tk.Label(left_frame, text="HTML File:").grid(row=0, column=0, sticky="e")
        self.html_path = tk.Entry(left_frame, width=40)
        self.html_path.grid(row=0, column=1, padx=5, pady=5)
        tk.Button(left_frame, text="Browse", command=lambda: self.select_file(self.html_path, "html")).grid(row=0, column=2)
        tk.Label(left_frame, text="CSS File:").grid(row=1, column=0, sticky="e")
        self.css_path = tk.Entry(left_frame, width=40)
        self.css_path.grid(row=1, column=1, padx=5, pady=5)
        tk.Button(left_frame, text="Browse", command=lambda: self.select_file(self.css_path, "css")).grid(row=1, column=2)
        tk.Label(left_frame, text="Output PDF:").grid(row=2, column=0, sticky="e")
        self.output_pdf = tk.Entry(left_frame, width=40)
        self.output_pdf.grid(row=2, column=1, padx=5, pady=5)
        tk.Button(left_frame, text="Browse", command=lambda: self.select_file(self.output_pdf, "pdf", save=True)).grid(row=2, column=2)
        self.generate_button = tk.Button(left_frame, text="Generate PDF", command=self.generate_pdf_action)
        self.generate_button.grid(row=3, columnspan=3, pady=10)
        self.total_pages_label = tk.Label(left_frame, text="Total Pages: 0", font=("Times New Roman", 12, "bold"))
        self.total_pages_label.grid(row=6, column=1, pady=10)
        self.total_pages_label.grid_remove()
        right_frame = tk.Frame(root)
        right_frame.grid(row=0, column=1, sticky="nsew", padx=10, pady=10) 
        right_frame.rowconfigure(0, weight=1)
        right_frame.columnconfigure(0, weight=1)
        self.canvas = tk.Canvas(right_frame)
        self.scrollbar = ttk.Scrollbar(right_frame, orient="vertical", command=self.canvas.yview)
        self.scrollable_frame = ttk.Frame(self.canvas)
        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        )
        self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        self.canvas.configure(yscrollcommand=self.scrollbar.set)
        self.canvas.grid(row=0, column=0, sticky="nsew")
        self.scrollbar.grid(row=0, column=1, sticky="ns")
        if os.name == 'nt':
            self.canvas.bind("<MouseWheel>", self._on_mousewheel)
        elif os.name == 'posix':
            self.canvas.bind("<Button-4>", self._on_mousewheel)  # For Linux systems
            self.canvas.bind("<Button-5>", self._on_mousewheel)  # For Linux systems

        self.auto_select_files()


    def auto_select_files(self):
        """Automatically selects the first available HTML, CSS, and PDF files in the current directory."""
        current_dir = os.getcwd()
        files = os.listdir(current_dir)
        html_files = sorted([f for f in files if f.lower().endswith(".html")])
        css_files = sorted([f for f in files if f.lower().endswith(".css")])
        pdf_files = sorted([f for f in files if f.lower().endswith(".pdf")])
        
        if html_files:
            html_path = os.path.abspath(os.path.join(current_dir, html_files[0]))
            self.html_path.delete(0, tk.END)
            self.html_path.insert(0, html_path)
            
        if css_files:
            css_path = os.path.abspath(os.path.join(current_dir, css_files[0]))
            self.css_path.delete(0, tk.END)
            self.css_path.insert(0, css_path)
            
        if pdf_files:
            pdf_path = os.path.abspath(os.path.join(current_dir, pdf_files[0]))
        else:
            pdf_path = os.path.abspath(os.path.join(current_dir, "RESUME.pdf"))

        self.output_pdf.delete(0, tk.END)
        self.output_pdf.insert(0, pdf_path)

    def select_file(self, entry_widget, file_type="html", save=False):
        filetypes = {
            "html": [("HTML files", "*.html"), ("All files", "*.*")],
            "css": [("CSS files", "*.css"), ("All files", "*.*")],
            "pdf": [("PDF files", "*.pdf"), ("All files", "*.*")],
        }
        initial_dir = os.getcwd()
        if save:
            file_path = filedialog.asksaveasfilename(
                defaultextension=f".{file_type}",
                filetypes=filetypes[file_type],
                initialdir=initial_dir
            )
        else:
            file_path = filedialog.askopenfilename(
                filetypes=filetypes[file_type],
                initialdir=initial_dir
            )
        if file_path:
            entry_widget.delete(0, tk.END)
            entry_widget.insert(0, file_path)

    def generate_pdf_action(self):
        html_file = self.html_path.get()
        css_file = self.css_path.get()
        output_file = self.output_pdf.get()
        if not html_file or not css_file or not output_file:
            messagebox.showerror("Error", "Please select HTML, CSS, and output file.")
            return
        self.generate_button.config(state="disabled")

        try:
            self.run_generate_pdf(html_file, css_file, output_file)
        except Exception as e:
            messagebox.showerror("Error", f"An error occurred: {str(e)}")
        finally:
            self.generate_button.config(state="normal")

    def run_generate_pdf(self, html_file, css_file, output_file):
        asyncio.run(self.async_generate_pdf(html_file, css_file, output_file))
        if os.path.exists(output_file):
            with fitz.open(output_file) as pdf_document:
                total_pages = len(pdf_document)
            self.update_total_pages_label(total_pages)
            self.display_pdf_in_gui(output_file)
            messagebox.showinfo("Success", "PDF generated successfully!")
        else:
            messagebox.showerror("Error", "Failed to generate PDF. Output file not found.")

    def update_total_pages_label(self, total_pages):
        if hasattr(self, "total_pages_label"):
            self.total_pages_label.grid()
            self.total_pages_label.config(text=f"Total Pages: {total_pages}")
        else:
            self.total_pages_label = ttk.Label(self.root, text=f"Total Pages: {total_pages}", font=("Arial", 12, "bold"))

    async def async_generate_pdf(self, html_file, css_file, output_file):
        """Generates a PDF from HTML and CSS using headless Chrome and fitz/PyMuPDF."""
        try:
            browser = await launch(
                executablePath=r"C:\Program Files\Google\Chrome\Application\chrome.exe",
                args=['--no-sandbox', '--disable-setuid-sandbox'],
                headless=True
            )
            page = await browser.newPage()
            with open(html_file, 'r', encoding='utf-8') as file:
                html_content = file.read()
            with open(css_file, 'r', encoding='utf-8') as file:
                css_content = file.read()
            html_with_css = f"<style>{css_content}</style>{html_content}"
            await page.setContent(html_with_css)
            await page.pdf({
                'path': output_file,
                'format': 'A4',
                'printBackground': True
            })
            await browser.close()
            self.root.after(0, self.preview_pdf, output_file)
            if os.path.exists(output_file):
                pdf_document = fitz.open(output_file)
                total_pages = len(pdf_document)
                pdf_document.close()
                self.root.after(0, self.update_total_pages_label, total_pages)
        except Exception as e:
            self.root.after(0, messagebox.showerror, "Error", f"Failed to generate PDF: {e}")

    def preview_pdf(self, file_path):
        """Display the generated PDF in the GUI."""
        if os.path.exists(file_path):
            try:
                self.display_pdf_in_gui(file_path)
            except Exception as e:
                messagebox.showerror("Error", f"Failed to display PDF: {e}")
        else:
            messagebox.showerror("Error", "PDF file not found. Generate it first.")

    def display_pdf_in_gui(self, file_path):
        for widget in self.scrollable_frame.winfo_children():
            widget.destroy()
        doc = fitz.open(file_path)
        for page_num in range(len(doc)):
            page = doc.load_page(page_num)
            pixmap = page.get_pixmap(matrix=fitz.Matrix(2.0, 2.0))
            img = Image.frombytes("RGB", [pixmap.width, pixmap.height], pixmap.samples)
            photo = ImageTk.PhotoImage(img)
            label = tk.Label(self.scrollable_frame, image=photo)
            label.image = photo
            label.pack(pady=10)
            page_label = tk.Label(self.scrollable_frame, text=f"Page {page_num + 1}")
            page_label.pack()
        doc.close()

    def _on_mousewheel(self, event):
        if event.num == 5 or event.delta < 0:
            self.canvas.yview_scroll(1, "units")
        elif event.num == 4 or event.delta > 0:
            self.canvas.yview_scroll(-1, "units")

if __name__ == "__main__":
    root = tk.Tk()
    app = PDFGeneratorApp(root)
    root.mainloop()
