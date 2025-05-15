# Copyright Matthew Boiarski 2025 #

# Core Imports #
import subprocess
import sys

### AUTO IMPORT SECTION ###
required_packages = {
    'os': None,
    'subprocess': None,
    'asyncio': None,
    'selenium': 'selenium',
    'webdriver_manager' : 'webdriver_manager',
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

from urllib.request import pathname2url

class PDFGeneratorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("PDF Generator")

        # Configure root window to be resizable
        self.root.rowconfigure(0, weight=1)  # Main row for content
        self.root.columnconfigure(1, weight=1)  # Right-hand side (preview) expands

        # Left frame for input fields and buttons
        left_frame = tk.Frame(root)
        left_frame.grid(row=0, column=0, sticky="ns", padx=10, pady=10)  # Sticky "ns" to stretch vertically

        tk.Label(left_frame, text="HTML File:").grid(row=0, column=0, sticky="e")
        self.html_path = tk.Entry(left_frame, width=40)
        self.html_path.grid(row=0, column=1, padx=5, pady=5)
        tk.Button(left_frame, text="Browse", command=lambda: self.select_file(self.html_path)).grid(row=0, column=2)

        tk.Label(left_frame, text="CSS File:").grid(row=1, column=0, sticky="e")
        self.css_path = tk.Entry(left_frame, width=40)
        self.css_path.grid(row=1, column=1, padx=5, pady=5)
        tk.Button(left_frame, text="Browse", command=lambda: self.select_file(self.css_path)).grid(row=1, column=2)

        tk.Label(left_frame, text="Output PDF:").grid(row=2, column=0, sticky="e")
        self.output_pdf = tk.Entry(left_frame, width=40)
        self.output_pdf.grid(row=2, column=1, padx=5, pady=5)
        tk.Button(left_frame, text="Browse", command=lambda: self.select_file(self.output_pdf)).grid(row=2, column=2)

        self.generate_button = tk.Button(left_frame, text="Generate PDF", command=self.generate_pdf_action)
        self.generate_button.grid(row=3, columnspan=3, pady=10)

        # Add the print button right after the generate button
        self.print_button = tk.Button(left_frame, text="Print", command=self.print_pdf)
        self.print_button.grid(row=4, columnspan=3, pady=10)
        self.print_button.grid_remove()

        # Create the total pages label
        self.total_pages_label = tk.Label(left_frame, text="Total Pages: 0", font=("Arial", 12, "bold"))
        self.total_pages_label.grid(row=6, column=1, pady=10)  # Place it directly under the upload button
        self.total_pages_label.grid_remove()

        # Right frame for PDF preview
        right_frame = tk.Frame(root)
        right_frame.grid(row=0, column=1, sticky="nsew", padx=10, pady=10)  # Sticky "nsew" to stretch in all directions

        # Configure right frame to expand with resizing
        right_frame.rowconfigure(0, weight=1)
        right_frame.columnconfigure(0, weight=1)

        # Canvas and scrollbar for preview area
        self.canvas = tk.Canvas(right_frame)
        self.scrollbar = ttk.Scrollbar(right_frame, orient="vertical", command=self.canvas.yview)

        # Scrollable frame inside the canvas
        self.scrollable_frame = ttk.Frame(self.canvas)

        # Bind the scrollable frame to the canvas
        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        )

        # Create a window inside the canvas for the scrollable frame
        self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")

        # Configure canvas to work with scrollbar
        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        # Pack canvas and scrollbar into the right frame
        self.canvas.grid(row=0, column=0, sticky="nsew")
        self.scrollbar.grid(row=0, column=1, sticky="ns")

        # Bind mousewheel events for scrolling
        if os.name == 'nt':  # Windows
            self.canvas.bind("<MouseWheel>", self._on_mousewheel)
        elif os.name == 'posix':  # Linux/macOS
            self.canvas.bind("<Button-4>", self._on_mousewheel)  # For Linux systems
            self.canvas.bind("<Button-5>", self._on_mousewheel)  # For Linux systems

        self.auto_select_files()


    def auto_select_files(self):
        """Automatically selects the first available HTML, CSS, and PDF files in the current directory."""
        current_dir = os.getcwd()

        # Get lists of files with specific extensions
        html_files = [f for f in os.listdir(current_dir) if f.endswith(".html")]
        css_files = [f for f in os.listdir(current_dir) if f.endswith(".css")]
        pdf_files = [f for f in os.listdir(current_dir) if f.endswith(".pdf")]

        # Automatically select the first HTML file, if available
        if html_files:
            self.html_path.delete(0, tk.END)
            self.html_path.insert(0, os.path.join(current_dir, html_files[0]))

        # Automatically select the first CSS file, if available
        if css_files:
            self.css_path.delete(0, tk.END)
            self.css_path.insert(0, os.path.join(current_dir, css_files[0]))

        # Automatically select the first PDF file or set a default name
        if pdf_files:
            self.output_pdf.delete(0, tk.END)
            self.output_pdf.insert(0, os.path.join(current_dir, pdf_files[0]))
        else:
            # Default to "RESUME.pdf" in the current directory
            self.output_pdf.delete(0, tk.END)
            self.output_pdf.insert(0, os.path.join(current_dir, "RESUME.pdf"))


    def select_file(self, var, file_type, save=False):
        filetypes = {
            "html": [("HTML files", "*.html"), ("All files", "*.*")],
            "css": [("CSS files", "*.css"), ("All files", "*.*")],
            "pdf": [("PDF files", "*.pdf"), ("All files", "*.*")],
        }

        if save:
            file_path = filedialog.asksaveasfilename(defaultextension=f".{file_type}", filetypes=filetypes[file_type])
        else:
            file_path = filedialog.askopenfilename(filetypes=filetypes[file_type])

        if file_path:
            var.set(file_path)

    def generate_pdf_action(self):
        html_file = self.html_path.get()
        css_file = self.css_path.get()
        output_file = self.output_pdf.get()

        if not html_file or not css_file or not output_file:
            messagebox.showerror("Error", "Please select HTML, CSS, and output file.")
            return

        # Disable the generate button to prevent multiple clicks
        self.generate_button.config(state="disabled")

        try:
            # Run the PDF generation synchronously
            self.run_generate_pdf(html_file, css_file, output_file)
        except Exception as e:
            # Show error message if an exception occurs
            messagebox.showerror("Error", f"An error occurred: {str(e)}")
        finally:
            # Re-enable the generate button
            self.generate_button.config(state="normal")


    def run_generate_pdf(self, html_file, css_file, output_file):
        # Run the asynchronous function synchronously
        asyncio.run(self.async_generate_pdf(html_file, css_file, output_file))

        # Update total pages label after generating the PDF
        if os.path.exists(output_file):
            with fitz.open(output_file) as pdf_document:
                total_pages = len(pdf_document)

            # Update the total pages label
            self.update_total_pages_label(total_pages)

            # Display the generated PDF
            self.display_pdf_in_gui(output_file)

            # Show success message
            messagebox.showinfo("Success", "PDF generated successfully!")
            self.print_button.grid()
        else:
            messagebox.showerror("Error", "Failed to generate PDF. Output file not found.")


    def update_total_pages_label(self, total_pages):
        """Updates the total page count in the UI."""
        if hasattr(self, "total_pages_label"):
            self.total_pages_label.grid()
            self.total_pages_label.config(text=f"Total Pages: {total_pages}")
        else:
            self.total_pages_label = ttk.Label(self.root, text=f"Total Pages: {total_pages}", font=("Arial", 12, "bold"))

    async def async_generate_pdf(self, html_file, css_file, output_file):
        """Generates a PDF from HTML and CSS using headless Chrome."""


        try:
            browser = await launch(
                executablePath=r"C:\Program Files\Google\Chrome\Application\chrome.exe",
                args=['--no-sandbox', '--disable-setuid-sandbox'],
                headless=True
            )
            page = await browser.newPage()

            # Construct file:// URL for HTML
            file_url = f'file:///{pathname2url(os.path.abspath(html_file))}'

            # Load the HTML file directly
            await page.goto(file_url)

            # Inject CSS via <style> tag
            with open(css_file, 'r', encoding='utf-8') as file:
                css_content = file.read()
            await page.addStyleTag({'content': css_content})

            # Generate the PDF
            await page.pdf({
                'path': output_file,
                'format': 'A4',
                'printBackground': True
            })
            await browser.close()

            # Update UI
            self.root.after(0, self.preview_pdf, output_file)
            if os.path.exists(output_file):
                with fitz.open(output_file) as pdf_document:
                    total_pages = len(pdf_document)
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
        # Clear previous images and labels in the scrollable frame
        for widget in self.scrollable_frame.winfo_children():
            widget.destroy()

        # Open the PDF file
        doc = fitz.open(file_path)
        for page_num in range(len(doc)):
            page = doc.load_page(page_num)
            pixmap = page.get_pixmap(matrix=fitz.Matrix(2.0, 2.0))  # High-quality rendering
            img = Image.frombytes("RGB", [pixmap.width, pixmap.height], pixmap.samples)
            photo = ImageTk.PhotoImage(img)

            # Add each page as an image to the scrollable frame
            label = tk.Label(self.scrollable_frame, image=photo)
            label.image = photo  # Keep a reference to avoid garbage collection
            label.pack(pady=10)

            # Add a label for the page number
            page_label = tk.Label(self.scrollable_frame, text=f"Page {page_num + 1}")
            page_label.pack()

        doc.close()

    def _on_mousewheel(self, event):
        """Handle mousewheel scrolling.""" 
        if event.num == 5 or event.delta < 0:
            self.canvas.yview_scroll(1, "units")
        elif event.num == 4 or event.delta > 0:
            self.canvas.yview_scroll(-1, "units")

    def print_pdf(self):
        output_file = os.path.normpath(self.output_pdf.get())

        if os.path.exists(output_file):
            if os.name == "nt":  # Windows
                # Open the PDF file with the default viewer (this will allow printing)
                try:
                    os.startfile(output_file, "print")
                except Exception as e:
                    messagebox.showerror("Error", f"Failed to open PDF for printing: {e}")
            elif os.name == "posix":  # Linux
                # Open the PDF file with the default viewer (this will allow printing)
                try:
                    subprocess.run(["xdg-open", output_file], check=True)
                except subprocess.CalledProcessError:
                    messagebox.showerror("Error", "Failed to open PDF for printing.")
            else:
                messagebox.showerror("Error", "Unsupported OS for printing.")
        else:
            messagebox.showerror("Error", "PDF file not found. Generate it first.")


# Run the app
if __name__ == "__main__":
    root = tk.Tk()
    app = PDFGeneratorApp(root)
    root.mainloop()
