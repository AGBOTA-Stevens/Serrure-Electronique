from django.apps import AppConfig
  

class IotSerrureConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'iot_serrure'

    def ready(self):
        import iot_serrure.signals  # 👈 important
