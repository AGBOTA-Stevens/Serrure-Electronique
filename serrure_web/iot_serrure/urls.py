from . import views
from django.urls import path
app_name = 'iot_serrure'
urlpatterns = [
    path('', views.home, name='home'),  # Home view
    path('connexion/', views.connexion, name='connexion'),  # Login view
    path('api/verifier_badge_pass/', views.verifier_badge_pass, name='verifier_badge_pass'),  # Badge pass verification
    path('dashboard/<int:user_id>/', views.dashboard, name='dashboard'),  # Dashboard view
    path('logout/', views.logout_view, name='logout'),  # Logout view
    path('password_reset/', views.password_reset_request, name='password_reset'),  # Password reset request
    path('reset/<uidb64>/<token>/', views.password_reset_confirm, name='password_reset_confirm'),  # Password reset confirmation
    path('api/activer_badge_pass/', views.activer_badge_pass, name='activer_badge_pass'),  # Activate badge pass
    path('404/', views.handler404, name='handler404'),  # Custom 404 error handler
]