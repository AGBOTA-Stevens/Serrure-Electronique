from django.db import models
import unicodedata
import re
from django.contrib.auth.models import AbstractBaseUser, BaseUserManager, PermissionsMixin
from django.utils.crypto import get_random_string
from django.contrib.auth.models import Group, Permission
from django.utils import timezone

def normalize_string(value):
    # Supprimer accents → é -> e
    value = unicodedata.normalize('NFKD', value).encode('ascii', 'ignore').decode('utf-8')
    # Mettre en minuscules
    value = value.lower()
    # Remplacer espaces et caractères spéciaux par rien
    value = re.sub(r'[^a-z0-9]+', '', value)
    return value

class UserManager(BaseUserManager):
    def create_user(self, email, name, surname, password=None, **extra_fields):
        if not email:
            raise ValueError('Le champ email est requis.')
        if not name:
            raise ValueError('Le champ name est requis')
        if not surname:
            raise ValueError('Le champ surname est requis')
        email = self.normalize_email(email)
        user = self.model(email=email, surname=surname, name=name, **extra_fields)
        user.set_password(password)
        user.save(using=self._db)
        return user
    
    def create_superuser(self, email, name, surname, password=None, **extra_fields):
        extra_fields.setdefault('is_staff', True)
        extra_fields.setdefault('is_superuser', True)
        
        if extra_fields.get('is_staff') is not True:
            raise ValueError('Le super utilisateur doit avoir is_staff=True.')
        if extra_fields.get('is_superuser') is not True:
            raise ValueError('Le super utilisateur doit avoir is_superuser=True.')
            
        return self.create_user(email, name, surname, password, **extra_fields)


class Users(AbstractBaseUser, PermissionsMixin):
    name = models.CharField(max_length=150)
    surname = models.CharField(max_length=150)
    identifiant = models.CharField(max_length=150, unique=True, blank=True)
    email = models.EmailField(unique=True, max_length=150)
    password = models.CharField(max_length=255)
    code = models.CharField(max_length=5, blank=True)
    last_login = models.DateTimeField(auto_now_add=True)
    is_superuser = models.BooleanField(default=False)
    is_active = models.BooleanField(default=False)
    is_staff = models.BooleanField(default=True)
    badge_pass = models.CharField(max_length=50, unique=True, auto_created=True, blank=True)
    date_joined = models.DateTimeField(default=timezone.now)
    
    objects = UserManager()
    
    groupe = models.ManyToManyField(
        Group,
        related_name='users_groups',
        blank=True
    )
    user_permission = models.ManyToManyField(
        Permission,
        related_name='users_user_permissions',
        blank=True
    )
    def save(self, *args, **kwargs):
        if not self.badge_pass:
            self.badge_pass = get_random_string(length=50)
        if not self.identifiant:
            nom = normalize_string(self.name)
            prenom = normalize_string(self.surname)
            base_identifiant = f"{nom}.{prenom}"

            identifiant_unique = base_identifiant
            count = 1

            # Vérifie si identifiant existe déjà
            while Users.objects.filter(identifiant=identifiant_unique).exists():
                identifiant_unique = f"{base_identifiant}{count}"
                count += 1

            self.identifiant = identifiant_unique
        super().save(*args, **kwargs)
    USERNAME_FIELD = 'identifiant'
    REQUIRED_FIELDS = ['email', 'name', 'surname']
    
    def __str__(self):    
        return self.identifiant
# Create your models here.