import os
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

# Set up the Selenium driver
# Make sure you have the Chrome driver executable in your system PATH
driver = webdriver.Firefox()

# Navigate to a website
# driver.get('http://127.0.0.1:8080')
path = os.path.abspath("./range/index.html")
driver.get("file:///"+path)

# Wait for an element to be visible on the page (e.g., a button)
wait = WebDriverWait(driver, 10)
button = wait.until(EC.visibility_of_element_located(
    (By.XPATH, '//button[@id="btn"]')))

# Perform actions on the page
button.click()

# Retrieve information from the page
element = driver.find_element(By.XPATH, '//h1[@class="target"]')
print(element.text)

# Close the browser
driver.quit()
