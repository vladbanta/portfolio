# 🎬 MovieReviews Platform (ASP.NET MVC)

This is a complete movie review web platform developed as part of a university project using ASP.NET MVC. The app allows users to register, login, submit movie reviews, fan theories, contact the admin, and request new movie additions.

## 🔧 Features

- 👤 **Authentication & Authorization**:
  - Register/Login with role-based access (User/Admin)
  - Admins can manage content, users, and reviews

- 🎥 **Movies Section**:
  - Add, edit, delete movies (Admin)
  - View movie details, ratings, and submit reviews (User)

- 📚 **Fan Theories**:
  - Add and view fan-submitted theories for movies
  - Admin can approve/delete inappropriate content

- 📨 **Movie Requests**:
  - Users can request new movies to be added
  - Admins can view and manage these requests

- 📬 **Contact Page**:
  - Users can send messages to the site administrators
  - Admin can read all messages

- 🎨 **Custom Design**:
  - Responsive front-end with custom CSS
  - Integrated with Bootstrap and jQuery

## 🧰 Technologies Used

- C#  
- ASP.NET Core MVC (.NET 9.0)  
- Entity Framework Core  
- SQL Server  
- Identity for authentication  
- Bootstrap & jQuery  
- Razor Views

## 🗃️ Database Structure

- `AspNetUsers` - Registered users  
- `Movies` - Movie catalog  
- `Reviews` - User-submitted movie reviews  
- `FanTheories` - Theories submitted by users  
- `MovieRequests` - Requested new movies  
- `ContactMessages` - Messages sent via the Contact page

