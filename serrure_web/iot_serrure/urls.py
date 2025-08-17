from . import views
from django.urls import path
app_name = 'iot_serrure'
urlpatterns = [
    path('', views.home, name='home'),  # Home view
    path('send_badge_pass/', views.send_badge_pass, name='send_badge_pass'),
    path('connexion/', views.connexion, name='connexion'),  # Login view
    path('api/verifier_badge_pass/', views.verifier_badge_pass, name='verifier_badge_pass'),  # Badge pass verification
]