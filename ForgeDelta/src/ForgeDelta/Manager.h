#pragma once

namespace ForgeDelta {
  struct AppplicationData;
}

namespace ForgeDelta {
  class Manager {
  public:
    static Manager& GetInstance() {
      static Manager instance;
      return instance;
    }

    // Delete copy constructor and assignment operator to prevent copying
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    void SetApplicationData(ApplicationData* app) {
      m_ApplicationData = app;
    }

    ApplicationData* GetApplicationData() {
      return m_ApplicationData;
    }

  private:
    Manager() = default; // Private constructor
    ~Manager() = default; // Private destructor

    inline static ApplicationData* m_ApplicationData = nullptr;
  };

}