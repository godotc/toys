from selenium import webdriver
from selenium.webdriver.firefox.options import Options as fxopt
from selenium.webdriver.chrome.options import Options as chropt
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


def firefox():
    # Set up the Selenium driver with a custom Firefox profile
    profile = webdriver.FirefoxProfile()
    # Replace with the path to your MetaMask extension XPI file

    # profile.add_extension('/path/to/metamask/extension.xpi')

    options = fxopt()
    options.profile = profile

    driver = webdriver.Firefox(options=options)
    driver.get('https://www.example.com')

    # Wait for the MetaMask extension to load
    wait = WebDriverWait(driver, 10)
    connect_button = wait.until(EC.element_to_be_clickable(
        (By.XPATH, '//button[contains(text(), "Connect")]')))
    connect_button.click()

    # Wait for the MetaMask pop-up to appear
    wait.until(EC.number_of_windows_to_be(2))
    driver.switch_to.window(driver.window_handles[1])

    # Perform MetaMask actions (e.g., login, sign transactions)
    # ...

    # Switch back to the main window
    driver.switch_to.window(driver.window_handles[0])

    # Continue with your scraping or other actions on the website
    # ...

    # Close the browser
    driver.quit()


def chrome():
    # Set up the Selenium driver with MetaMask extension
    options = chropt()
    # Replace with the path to your MetaMask extension
    options.add_argument('--load-extension=./res/metamsk-pack/chrome/')

    driver = webdriver.Chrome(options=options)
    driver.get('https://www.example.com')

    # Wait for the MetaMask extension to load
    wait = WebDriverWait(driver, 10)
    connect_button = wait.until(EC.element_to_be_clickable(
        (By.XPATH, '//button[contains(text(), "Connect")]')))
    connect_button.click()

    # Wait for the MetaMask pop-up to appear
    wait.until(EC.frame_to_be_available_and_switch_to_it(
        (By.CSS_SELECTOR, 'iframe[src^="chrome-extension://"]')))

    # Switch to MetaMask's pop-up window
    metamask_window = driver.window_handles[-1]
    driver.switch_to.window(metamask_window)

    # Perform MetaMask actions (e.g., login, sign transactions)
    # ...

    # Switch back to the main window
    driver.switch_to.window(driver.window_handles[0])

    # Continue with your scraping or other actions on the website
    # ...

    # Close the browser
    driver.quit()


# firefox()
chrome()
