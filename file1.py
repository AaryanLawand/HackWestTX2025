import google.generativeai as genai

# Set your Gemini API key here
genai.configure(api_key="AIzaSyDS9T80KdwRTESpglHVEsADGWSknEA38rE")

# Initialize the Gemini model
model = genai.GenerativeModel('gemini-pro')

# Example input prompt
prompt = "Explain the concept of quantum entanglement in simple terms."

# Generate a response
response = model.generate_content(prompt)

# Print the output
print("Response:")
print(response.text)
